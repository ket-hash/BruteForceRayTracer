#ifndef INTERVAL_H
#define INTERVAL_H

class interval{
	public:
		float min, max;
	
	interval() : min(+infinity), max(-infinity) {}
	interval(float min, float max) : min(min), max(max) {}
	interval(const interval& a, const interval& b){
		max = a.max >= b.max ? a.max : b.max;
		min = a.min <= b.min ? a.min : b.min;
	}

	float size() const {
		return max - min;
	}

	bool contains(float x) const {
		return (x >= min && max >= x);
	}

	bool surrounds(float x) const {
		return (x > min && max > x);
	}

	float clamp(float x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	interval expand(float delta) const {
		auto padding = delta/2;
		return interval(min - padding, max + padding);
	}

	static const interval empty, universe;
};

const interval interval::empty = interval();
const interval interval::universe = interval(-infinity, +infinity);

#endif
