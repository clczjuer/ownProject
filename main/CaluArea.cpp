#include "CaluArea.h"


clc::CaluArea::CaluArea()
{
}

clc::CaluArea::~CaluArea()
{

}



list<int> clc::CaluArea::ProcessContours(Size imgSize, vector< vector<Point> > contours,
	int contourIdx, int maxLevel /*= INT_MAX*/)
{
	size_t ncontours = contours.size();
	size_t i = 0, first = 0, last = ncontours;
	vector<CvSeq> seq;
	vector<CvSeqBlock> block;

// 	if (!last)
// 		return 0;

	seq.resize(last);
	block.resize(last);

	for (i = first; i < last; i++)
		seq[i].first = 0;

	if (contourIdx >= 0)
	{
		CV_Assert(0 <= contourIdx && contourIdx < (int)last);
		first = contourIdx;
		last = contourIdx + 1;
	}

	for (i = first; i < last; i++)
	{
		Mat ci = ((cv::OutputArray)contours).getMat(int(i));
		if (ci.empty())
			continue;
		int npoints = ci.checkVector(2, CV_32S);
		CV_Assert(npoints > 0);
		cvMakeSeqHeaderForArray(CV_SEQ_POLYGON, sizeof(CvSeq), sizeof(Point),
			ci.data, npoints, &seq[i], &block[i]);
	}

	for (i = first; i < last; i++)
	{
		seq[i].h_next = i < last - 1 ? &seq[i + 1] : 0;
		seq[i].h_prev = i > first ? &seq[i - 1] : 0;
	}

	return cvProcessContours(imgSize, &seq[first], contourIdx >= 0 ?
		-maxLevel : maxLevel);
}



list<int> clc::CaluArea::cvProcessContours(Size imgSize, CvSeq* contour, int max_level)
{
	CvSeq *contour0 = contour, *h_next = 0;
	CvTreeNodeIterator iterator;
	cv::vector<clc::PolyEdge> edges;
	cv::vector<cv::Point> pts;
	cv::Point offset = Point(0, 0);
	int thickness = -1;
// 	if (!contour)
// 		return 0;
	list<int> area_list;
	max_level = MAX(max_level, INT_MIN + 2);
	max_level = MIN(max_level, INT_MAX - 1);

	if (max_level < 0)
	{
		h_next = contour->h_next;
		contour->h_next = 0;
		max_level = -max_level + 1;
	}

	cvInitTreeNodeIterator(&iterator, contour, max_level);
	while ((contour = (CvSeq*)cvNextTreeNode(&iterator)) != 0)
	{
		CvSeqReader reader;
		int i, count = contour->total;
		int elem_type = CV_MAT_TYPE(contour->flags);

		cvStartReadSeq(contour, &reader, 0);
		//if (thickness < 0)
		pts.resize(0);

		if (CV_IS_SEQ_CHAIN_CONTOUR(contour))
		{
			cv::Point pt = ((CvChain*)contour)->origin;
			cv::Point prev_pt = pt;
			char prev_code = reader.ptr ? reader.ptr[0] : '\0';

			prev_pt += offset;

			for (i = 0; i < count; i++)
			{
				char code;
				CV_READ_SEQ_ELEM(code, reader);

				assert((code & ~7) == 0);

				if (code != prev_code)
				{
					prev_code = code;
					pts.push_back(pt);
					prev_pt = pt;
				}

				pt.x += CodeDeltas[(int)code][0];
				pt.y += CodeDeltas[(int)code][1];
			}

			CollectPolyEdges(&pts[0], (int)pts.size(), edges, 0);
		}
		else if (CV_IS_SEQ_POLYLINE(contour))
		{
			CV_Assert(elem_type == CV_32SC2);
			cv::Point pt1, pt2;
			int shift = 0;

			count -= !CV_IS_SEQ_CLOSED(contour);
			CV_READ_SEQ_ELEM(pt1, reader);
			pt1 += offset;
			//if (thickness < 0)
			pts.push_back(pt1);

			for (i = 0; i < count; i++)
			{
				CV_READ_SEQ_ELEM(pt2, reader);
				pt2 += offset;
				
				pts.push_back(pt2);
				pt1 = pt2;
			}

			
		}
		CollectPolyEdges(&pts[0], (int)pts.size(), edges, 0);
		int area = 0;

		if (pts.size() <= 2)
		{
			area = 1;
		}
		else
		{
			area = CaculCollectionArea(imgSize, edges);
		}
		area_list.push_back(area);
		edges.clear();
	}


	if (h_next && contour0)
		contour0->h_next = h_next;
	return area_list;
}

list<int> clc::CaluArea::getArea(Size imgSize, vector< vector<Point> > contours, int contourIdx)
{
	return ProcessContours(imgSize, contours, contourIdx);
}




void clc::CollectPolyEdges(const Point* v, int count, vector<clc::PolyEdge>& edges, int shift)
{
	Point offset(0, 0);
	int i, delta = offset.y + (shift ? 1 << (shift - 1) : 0);
	Point pt0 = v[count - 1], pt1;
	pt0.x = (pt0.x + offset.x) << (XY_SHIFT - shift);
	pt0.y = (pt0.y + delta) >> shift;

	edges.reserve(edges.size() + count);

	for (i = 0; i < count; i++, pt0 = pt1)
	{
		Point t0, t1;
		PolyEdge edge;

		pt1 = v[i];
		pt1.x = (pt1.x + offset.x) << (XY_SHIFT - shift);
		pt1.y = (pt1.y + delta) >> shift;

		t0.y = pt0.y; t1.y = pt1.y;
		t0.x = (pt0.x + (XY_ONE >> 1)) >> XY_SHIFT;
		t1.x = (pt1.x + (XY_ONE >> 1)) >> XY_SHIFT;

		if (pt0.y == pt1.y)
			continue;

		if (pt0.y < pt1.y)
		{
			edge.y0 = pt0.y;
			edge.y1 = pt1.y;
			edge.x = pt0.x;
		}
		else
		{
			edge.y0 = pt1.y;
			edge.y1 = pt0.y;
			edge.x = pt1.x;
		}
		edge.dx = (pt1.x - pt0.x) / (pt1.y - pt0.y);
		edges.push_back(edge);
	}
}


int clc::CaculCollectionArea(Size imgSize, vector<PolyEdge>& edges)
{
	int area = 0;
	PolyEdge tmp;
	int i, y, total = (int)edges.size();
	PolyEdge* e;
	int y_max = INT_MIN, x_max = INT_MIN, y_min = INT_MAX, x_min = INT_MAX;
	
	if (total < 2)
	{
		return area;
	}

	for (i = 0; i < total; i++)
	{
		PolyEdge& e1 = edges[i];
		assert(e1.y0 < e1.y1);
		y_min = std::min(y_min, e1.y0);
		y_max = std::max(y_max, e1.y1);
		x_min = std::min(x_min, e1.x);
		x_max = std::max(x_max, e1.x);
	}

	if (y_max < 0 || y_min >= imgSize.height || x_max < 0 || x_min >= (imgSize.width << XY_SHIFT))
		return 0;

	std::sort(edges.begin(), edges.end(), CmpEdges());

	// start drawing
	tmp.y0 = INT_MAX;
	edges.push_back(tmp); // after this point we do not add
	// any elements to edges, thus we can use pointers
	i = 0;
	tmp.next = 0;
	e = &edges[i];
	y_max = MIN(y_max, imgSize.height);

	for (y = e->y0; y < y_max; y++)
	{
		PolyEdge *last, *prelast, *keep_prelast;
		int sort_flag = 0;
		int draw = 0;
		int clipline = y < 0;

		prelast = &tmp;
		last = tmp.next;
		while (last || e->y0 == y)
		{
			if (last && last->y1 == y)
			{
				// exclude edge if y reachs its lower point
				prelast->next = last->next;
				last = last->next;
				continue;
			}
			keep_prelast = prelast;
			if (last && (e->y0 > y || last->x < e->x))
			{
				// go to the next edge in active list
				prelast = last;
				last = last->next;
			}
			else if (i < total)
			{
				// insert new edge into active list if y reachs its upper point
				prelast->next = e;
				e->next = last;
				prelast = e;
				e = &edges[++i];
			}
			else
				break;

			if (draw)
			{
				if (!clipline)
				{
					// convert x's from fixed-point to image coordinates
					int x1 = keep_prelast->x;
					int x2 = prelast->x;

					if (x1 > x2)
					{
						int t = x1;

						x1 = x2;
						x2 = t;
					}

					x1 = (x1 + XY_ONE - 1) >> XY_SHIFT;
					x2 = x2 >> XY_SHIFT;

					// clip and draw the line
					if (x1 < imgSize.width && x2 >= 0)
					{
						if (x1 < 0)
							x1 = 0;
						if (x2 >= imgSize.width)
							x2 = imgSize.width - 1;
						area += x2 - x1 + 1;
					}
				}
				keep_prelast->x += keep_prelast->dx;
				prelast->x += prelast->dx;
			}
			draw ^= 1;
		}

		// sort edges (using bubble sort)
		keep_prelast = 0;

		do
		{
			prelast = &tmp;
			last = tmp.next;

			while (last != keep_prelast && last->next != 0)
			{
				PolyEdge *te = last->next;

				// swap edges
				if (last->x > te->x)
				{
					prelast->next = te;
					last->next = te->next;
					te->next = last;
					prelast = te;
					sort_flag = 1;
				}
				else
				{
					prelast = last;
					last = te;
				}
			}
			keep_prelast = prelast;
		} while (sort_flag && keep_prelast != tmp.next && keep_prelast != &tmp);
	}
	return area;
}



