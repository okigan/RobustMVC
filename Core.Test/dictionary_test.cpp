#include "stdafx.h"

#pragma managed(push, off)

#include <memory>
#include <boost/any.hpp>

#include <Core/dictionary.h>

#pragma managed(pop)


using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace CoreTest
{
	[TestClass]
	public ref class dictionary_test_suite
	{
	public: 
		[TestMethod]
		void dictionary_test()
		{
            dictionary spb;

            std::string key = "key";
            auto raw_in_value = 11;
            auto in_value = std::make_shared<int>(raw_in_value);

            spb.insert(key, raw_in_value);

            boost::any out_value;
            bool found = spb.find(key, out_value);

            Assert::AreEqual(true, found);

            auto raw_out_value = boost::any_cast<decltype(raw_in_value)>(out_value);

            Assert::AreEqual(raw_in_value, raw_out_value);

            Assert::AreEqual(1U, spb.size());

            found = spb.find("does_not_exist", out_value);

            Assert::AreEqual(false, found);

		}
	};
}
