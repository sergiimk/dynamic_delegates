//////////////////////////////////////////////////////////////////////////
#define BOOST_TEST_MODULE FileSystem test
#include <boost/test/unit_test.hpp>
#include "delegate.h"
//////////////////////////////////////////////////////////////////////////

struct Test
{
	int payload;
	Test() : payload(357) { }
};

struct Test2
{
	int do_stuff(Test t) { return t.payload + 1; }
};

void F1(Test t)
{
	BOOST_CHECK_EQUAL(t.payload, 357);
}

//////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( DelegateTestSuite );

BOOST_AUTO_TEST_CASE( TestStaticFunc )
{
	auto deleg = make_delegate(&F1);
	Test t;
	deleg(t);
}

BOOST_AUTO_TEST_CASE( TestMethod )
{
	Test2 inst;
	auto deleg = make_delegate(&inst, &Test2::do_stuff);

	Test t;
	BOOST_CHECK_EQUAL(deleg(t), 358);
}

BOOST_AUTO_TEST_CASE( TestStaticFuncDyn )
{
	auto deleg = make_delegate_dynamic(&F1);
	Test t;
	deleg(t);

	void* args[] = { &t };
	deleg.invoke(args, 0);
}

BOOST_AUTO_TEST_CASE( TestMethodDyn )
{
	Test2 inst;
	auto deleg = make_delegate_dynamic(&inst, &Test2::do_stuff);

	Test t;
	BOOST_CHECK_EQUAL(deleg(t), 358);

	int ret = 0;
	void* args[] = { &t };
	deleg.invoke(args, &ret);
	BOOST_CHECK_EQUAL(ret, 358);
}

BOOST_AUTO_TEST_SUITE_END();
