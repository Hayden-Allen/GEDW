#include "pch.h"

int main()
{
	// I mixed up lvalues and rvalues a couple times last lecture.
	// To reiterate:
	//		- lvalues are variables (they can appear on the LEFT of an assignment operation)
	//		- rvalues are things computed on the fly, whose data exists but isn't assigned to a variable

	// Also, std::move gives an rvalue reference to its argument, regardless of the type of that argument. So value => rvalue, lvalue => rvalue, and rvalue => rvalue

	// someone said that having T f() const means the value returned by the function is const.
	// if we wanted to do that, we would write const T f() const. 
	// The const following the method name means the object that the method belongs to will not change
	// as a result of the method.

	// use the element-wise constructor
	printf("a\n");
	math::Vec2<float> a(1.f, 2.f);
	// the add operator creates a new vec2 via the element-wise constructor and the compiler moves it straight into b. I'm making b an rvalue reference for later demonstration.
	printf("b\n");
	math::Vec2<float>&& b = a + 5;

	// in this case, c1 is assigned directly to the value returned by the add operator by the compiler - an optimization that prevents our move constructor from being called
	printf("\n\nc1\n");
	math::Vec2<float> c1(a + 5);

	// the element-wise constructor is called from the add operator, so we see that first. However, the compiler sees std::move and knows that we now want to operate specifically on an rvalue. So in this case, rather than making c2 point to the vector returned by the add operator (as it did for c1), the compiler calls the move constructor.
	printf("c2\n");
	math::Vec2<float> c2(std::move(a + 5));

	// here the COPY constructor is invoked, even though b is an rvalue reference. This is a weird quirk - NAMED rvalue references are treated as lvalue references. Therefore, b gets captured by the copy constructor, which accepts an lvalue reference.
	printf("c3\n");
	math::Vec2<float> c3(b);

	// here the MOVE constructor is invoked. Once again, b alone would be treated as an lvalue reference because it is a NAMED rvalue reference. The std::move function, however, returns an rvalue reference to its argument. This rvalue reference is truly temporary (i.e. NOT NAMED), and therefore the result of std::move(b) is correctly captured by the MOVE constructor
	printf("c4\n");
	math::Vec2<float> c4(std::move(b));



	// Constructors are ALWAYS invoked for the initial creation of an object. You can see that these variables, even though they use = instead of a constructor call, give the exact same output as above.
	printf("\n\nd1\n");
	math::Vec2<float> d1 = a + 5;
	printf("d2\n");
	math::Vec2<float> d2 = std::move(a + 5);
	printf("d3\n");
	math::Vec2<float> d3 = b;
	printf("d4\n");
	math::Vec2<float> d4 = std::move(b);



	// However, now that these variables have been given an initial value (and the mandatory initial constructor call is out of the way), the = operator CAN take on a different meaning.
	printf("\n\n2d1\n");
	d1 = d2;
	printf("2d2\n");
	d2 = std::move(d3);
	printf("2d3\n");
	d3 = d1 + d2;
	printf("2d4\n");
	d4 = std::move(d1 + d2);
	return 0;
}