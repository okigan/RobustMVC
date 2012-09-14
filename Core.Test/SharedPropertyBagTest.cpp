#include "stdafx.h"

#pragma managed(push, off)

#include <memory>
#include <boost/any.hpp>

#include <Core/SharedPropertyBag.h>

#pragma managed(pop)


using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace CoreTest
{
	[TestClass]
	public ref class SharedPropertyBagTest
	{
	public: 
		[TestMethod]
		void TestSharedPropertyBag()
		{
            SharedPropertyBag spb;

            std::string key = "key";
            auto raw_in_value = 11;
            auto in_value = std::make_shared<boost::any>(raw_in_value);

            spb.Put(key, in_value);

            std::shared_ptr<boost::any> out_value;
            bool found = spb.Get(key, out_value);

            Assert::AreEqual(true, found);

            auto raw_out_value = boost::any_cast<decltype(raw_in_value)>(*out_value);

            Assert::AreEqual(raw_in_value, raw_out_value);

            Assert::AreEqual(1U, spb.Size());

            found = spb.Get("does_not_exist", out_value);

            Assert::AreEqual(false, found);

		}
	};
}
