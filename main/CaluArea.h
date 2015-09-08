#include "cv.h"
#include <list>
using namespace cv;
using namespace std;
namespace clc
{
	struct PolyEdge
	{
		PolyEdge() : y0(0), y1(0), x(0), dx(0), next(0) {}
		//PolyEdge(int _y0, int _y1, int _x, int _dx) : y0(_y0), y1(_y1), x(_x), dx(_dx) {}

		int y0, y1;
		int x, dx;
		PolyEdge *next;
	};
	static const int CodeDeltas[8][2] =
	{ { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };

	struct CmpEdges
	{
		bool operator ()(const PolyEdge& e1, const PolyEdge& e2)
		{
			return e1.y0 - e2.y0 ? e1.y0 < e2.y0 :
				e1.x - e2.x ? e1.x < e2.x : e1.dx < e2.dx;
		}
	};
	enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1 << 12) - 256 };
	class CaluArea
	{
	public:

		CaluArea();
		~CaluArea();
		//·µ»Øqlist ´ýÀ©Õ¹
		list<int> getArea(Size imgSize, vector< vector<Point> > contours, int contourIdx);
		list<int> ProcessContours(Size imgSize, vector< vector<Point> > contours,
			int contourIdx,
			int maxLevel = INT_MAX);
		list<int> cvProcessContours(Size imgSize, CvSeq* contour,
			int max_level);
	};


	static void CollectPolyEdges(const Point* v, int count, vector<clc::PolyEdge>& edges, int shift);
	static int CaculCollectionArea(Size imgSize, vector<PolyEdge>& edges);
}
