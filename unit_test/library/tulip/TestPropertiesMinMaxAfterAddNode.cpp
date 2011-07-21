/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include "TestPropertiesMinMaxAfterAddNode.h"

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

#include <cppunit/extensions/HelperMacros.h>
#include <tulip/LayoutProperty.h>
CPPUNIT_TEST_SUITE_REGISTRATION( TestPropertiesMinMaxAfterAddNode );

int i1 = 3;
int i2 = 56;

double d1 = 3.5;
double d2 = 89.6;

using namespace tlp;

void TestPropertiesMinMaxAfterAddNode::setUp() {
  graph = tlp::newGraph();
}

void TestPropertiesMinMaxAfterAddNode::tearDown() {
  delete graph;
}

void TestPropertiesMinMaxAfterAddNode::testDoublePropertyMinMaxAfterAddNode() {
  DoubleProperty *doubleProp = graph->getProperty<DoubleProperty>("doubleProp");

  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  // set values to doubleProp
  doubleProp->setNodeValue(n1, d1);
  doubleProp->setNodeValue(n2, d2);
  CPPUNIT_ASSERT_EQUAL(doubleProp->getNodeMin(graph), d1);
  CPPUNIT_ASSERT_EQUAL(doubleProp->getNodeMax(graph), d2);

  // add a new node, the value associated to doubleProp property is the default one 0
  node n3 = graph->addNode();

  // min should be 0
  CPPUNIT_ASSERT_EQUAL(doubleProp->getNodeMin(graph), 0.0d);
  CPPUNIT_ASSERT_EQUAL(doubleProp->getNodeMax(graph), d2);
}

void TestPropertiesMinMaxAfterAddNode::testIntegerPropertyMinMaxAfterAddNode() {
  IntegerProperty *intProp = graph->getProperty<IntegerProperty>("intProp");

  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  // set values to doubleProp
  intProp->setNodeValue(n1, i1);
  intProp->setNodeValue(n2, i2);
  CPPUNIT_ASSERT_EQUAL(intProp->getNodeMin(graph), i1);
  CPPUNIT_ASSERT_EQUAL(intProp->getNodeMax(graph), i2);

  // add a new node, the value associated to doubleProp property is the default one 0
  node n3 = graph->addNode();

  // min should be 0
  CPPUNIT_ASSERT_EQUAL(intProp->getNodeMin(graph), 0);
  CPPUNIT_ASSERT_EQUAL(intProp->getNodeMax(graph), i2);
}

void TestPropertiesMinMaxAfterAddNode::testLayoutPropertyMinMaxAfterAddNode() {
  LayoutProperty *property = graph->getProperty<LayoutProperty>("testLayout");
  
  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  Coord firstNodePos(1.0f, 2.0f);
  Coord secondNodePos(3.0f, 4.0f);
  
  // set values to doubleProp
  property->setNodeValue(n1, firstNodePos);
  property->setNodeValue(n2, secondNodePos);
  CPPUNIT_ASSERT_EQUAL(property->getMin(graph), firstNodePos);
  CPPUNIT_ASSERT_EQUAL(property->getMax(graph), secondNodePos);
  
  // add a new node, the value associated to doubleProp property is the default one 0
  node n3 = graph->addNode();
  
  // min should be 0
  CPPUNIT_ASSERT_EQUAL(property->getMin(graph), Coord());
  CPPUNIT_ASSERT_EQUAL(property->getMax(graph), secondNodePos);
}

CppUnit::Test *TestPropertiesMinMaxAfterAddNode::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Properties Min Max after addNode tests" );
  suiteOfTests->addTest( new CppUnit::TestCaller<TestPropertiesMinMaxAfterAddNode>( "DoubleProperty Min Max after addNode test",
                         &TestPropertiesMinMaxAfterAddNode::testDoublePropertyMinMaxAfterAddNode));
  suiteOfTests->addTest( new CppUnit::TestCaller<TestPropertiesMinMaxAfterAddNode>( "IntegerProperty Min Max after addNode test",
                         &TestPropertiesMinMaxAfterAddNode::testIntegerPropertyMinMaxAfterAddNode));
  suiteOfTests->addTest( new CppUnit::TestCaller<TestPropertiesMinMaxAfterAddNode>( "LayoutProperty Min Max after addNode test",
                         &TestPropertiesMinMaxAfterAddNode::testLayoutPropertyMinMaxAfterAddNode));
  return suiteOfTests;
}
