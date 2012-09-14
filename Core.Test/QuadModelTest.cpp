#include "stdafx.h"

#pragma managed(push, off)

#include <Core/Model/QuadModel.h>
#include <Core/Controller/QuadModelController.h>

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

        void fff(const Model::callback_params & params) 
        {
            called = true;
        }

        bool called;
    };

	[TestClass]
	public ref class QuadModelTest
	{
	public: 
		[TestMethod]
		void TestRadius()
		{
            QuadModel quadModel;

            double radius = 10;

            int stamp = quadModel.GetChangeStamp();

            quadModel.SetRadius(radius);

            Assert::AreEqual(radius, quadModel.GetRadius());

            Assert::AreNotEqual(stamp, quadModel.GetChangeStamp());
            Assert::AreEqual(quadModel.GetChangeStamp(), quadModel.GetChangeStamp());

		}



        [TestMethod]
        void TestRadiusCallback()
        {
            QuadModel quadModel;


            xxx x;

            QuadModel::Callback callback = std::bind(&xxx::fff, &x, std::placeholders::_1);

            quadModel.SetCallback(callback);
            
            double radius = 10;

            quadModel.SetRadius(radius);

            Assert::AreEqual(true, x.called);
        }

        [TestMethod]
        void TestContoller()
        {
            QuadModel model;
            QuadModelController controller;

            controller.SetModel(&model);

            double radius = model.GetRadius();

            controller.IncreaseRadius();

            Assert::IsTrue(radius < model.GetRadius());

            controller.DecreaseRadius();

            Assert::AreEqual(radius, model.GetRadius());
        }
	};
}
