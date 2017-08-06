#pragma once

template<typename T>
class DoubleBuffer
{
public:
	DoubleBuffer():front_(&a_),back_(&b_){}
	void swap() { T*t=front_;front_=back_;back_=t;; }
	T& front() const { return *front_; }
	T& back() const { return *back_; }
private:
	T a_, b_;
	T *front_, *back_;
};
