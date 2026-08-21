// What this package exists for: an arc that goes into a 2D boolean has to come
// back out as an arc. Clipper itself cannot do that -- it works on an integer
// lattice, so anything curved that reaches it returns as the chords it was
// walked along. libarea keeps arcs symbolic on the way in and re-fits them on
// the way out, and this checks both ends of that, plus that the boolean in
// between took away the area it should have.

#include <libarea/Area.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
	// A square with a round hole in it. The hole is two half circles, which is
	// how libarea wants an arc bigger than a semicircle given to it.
	CArea square_with_round_hole() {
		CArea a;

		CCurve outer;
		outer.append(CVertex(Point(0, 0)));
		outer.append(CVertex(Point(100, 0)));
		outer.append(CVertex(Point(100, 100)));
		outer.append(CVertex(Point(0, 100)));
		outer.append(CVertex(Point(0, 0)));
		a.append(outer);

		CCurve hole;
		// Wound against the outer contour, which is what makes it a hole
		// rather than a second solid: GetArea() adds up the curves as they are
		// given and does not work the nesting out for itself.
		hole.append(CVertex(Point(70, 50)));
		hole.append(CVertex(-1, Point(30, 50), Point(50, 50)));
		hole.append(CVertex(-1, Point(70, 50), Point(50, 50)));
		a.append(hole);

		return a;
	}

	CArea corner_square() {
		CArea a;
		CCurve c;
		c.append(CVertex(Point(95, 95)));
		c.append(CVertex(Point(105, 95)));
		c.append(CVertex(Point(105, 105)));
		c.append(CVertex(Point(95, 105)));
		c.append(CVertex(Point(95, 95)));
		a.append(c);
		return a;
	}

	int count_arcs(const CArea& a) {
		int n = 0;
		for (const auto& curve : a.m_curves) {
			for (const auto& v : curve.m_vertices) {
				if (v.m_type != 0) {
					++n;
				}
			}
		}
		return n;
	}
}

int main() {
	CArea a = square_with_round_hole();

	const double expected_before = 100. * 100. - M_PI * 20. * 20.;
	const double before = std::fabs(a.GetArea());
	if (std::fabs(before - expected_before) > 1.) {
		std::printf("area before: got %.4f, want %.4f\n", before, expected_before);
		return 1;
	}

	a.Subtract(corner_square());

	// The tool overlaps the square in a 5 by 5 corner and nothing else.
	const double expected_after = expected_before - 25.;
	const double after = std::fabs(a.GetArea());
	if (std::fabs(after - expected_after) > 1.) {
		std::printf("area after: got %.4f, want %.4f\n", after, expected_after);
		return 1;
	}

	// The point of the exercise. Without FitArcs this is 0 and the hole comes
	// back as a few hundred straight segments.
	const int arcs = count_arcs(a);
	if (arcs < 1) {
		std::printf("the round hole came back with %d arc vertices\n", arcs);
		return 1;
	}

	std::printf("ok: %.4f -> %.4f, %d arc vertices survived\n", before, after, arcs);
	return 0;
}
