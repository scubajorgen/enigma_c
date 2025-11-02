#ifndef TESTFRAME_H
#define TESTFRAME_H

#define TEST_NAME_LENGTH    20

void testSessionStart       ();
void testSessionWrapUp      ();
void moduleTestStart        (char* name);
void moduleTestWrapUp       ();
void testStart              (char* name);
void testWrapUp             ();

void assertStringEquals     (char* expected, char* result);
void assertIntIsNull        (int* result);
void assertIntEquals        (int expected, int result);
void assertLongNotEquals    (long notExpected, long result);
void assertFloatEquals      (float expected, float result);

#endif