#include "stdafx.h"

#pragma managed(push, off)

#include <Core/Model/quad_model.h>
#include <Core/Controller/quad_model_controller.h>

#pragma managed(pop)


using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace CoreTest
{
    struct xxx
    {
        xxx()
        {
            called = false;
        }

        void fff(const model::callback_params & params) 
        {
            called = true;
        }

        bool called;
    };

	[TestClass]
	public ref class quad_model_suite
	{
	public: 
		[TestMethod]
		void radius_test()
		{
            quad_model quad_model;

            double radius = 10;

            int stamp = quad_model.GetChangeStamp();

            quad_model.set_radius(radius);

            Assert::AreEqual(radius, quad_model.get_radius());

            Assert::AreNotEqual(stamp, quad_model.GetChangeStamp());
            Assert::AreEqual(quad_model.GetChangeStamp(), quad_model.GetChangeStamp());

		}



        [TestMethod]
        void radius_callback_test()
        {
            quad_model quad_model;


            xxx x;

            quad_model::Callback callback = std::bind(&xxx::fff, &x, std::placeholders::_1);

            quad_model.SetCallback(callback);
            
            double radius = 10;

            quad_model.set_radius(radius);

            Assert::AreEqual(true, x.called);
        }

        [TestMethod]
        void controller_test()
        {
            quad_model model;
            quad_model_controller controller;

            controller.set_model(&model);

            double radius = model.get_radius();

            controller.increase_radius();

            Assert::IsTrue(radius < model.get_radius());

            controller.decrease_radius();

            Assert::AreEqual(radius, model.get_radius());
        }
	};
}
