/* libnih
 *
 * test_timer.c - test suite for nih/timer.c
 *
 * Copyright © 2009 Scott James Remnant <scott@netsplit.com>.
 * Copyright © 2009 Canonical Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <nih/test.h>

#include <time.h>
#include <string.h>

#include <nih/macros.h>
#include <nih/list.h>
#include <nih/timer.h>


static int callback_called = 0;
static void *last_data;
static NihTimer *last_timer;

static void
my_callback (void *data, NihTimer *timer)
{
	callback_called++;
	last_data = data;
	last_timer = timer;
}

void
test_add_timeout (void)
{
	NihTimer *timer;
	time_t    t1, t2;

	/* Check that we can add a timeout function and that the structure
	 * returned is correctly populated and placed in the timers list.
	 */
	TEST_FUNCTION ("nih_timer_add_timeout");
	nih_timer_poll ();
	TEST_ALLOC_FAIL {
		t1 = time (NULL);
		timer = nih_timer_add_timeout (NULL, 10, my_callback, &timer);
		t2 = time (NULL);

		if (test_alloc_failed) {
			TEST_EQ_P (timer, NULL);
			continue;
		}

		TEST_ALLOC_SIZE (timer, sizeof (NihTimer));
		TEST_LIST_NOT_EMPTY (&timer->entry);
		TEST_EQ (timer->type, NIH_TIMER_TIMEOUT);
		TEST_GE (timer->due, t1 + 10);
		TEST_LE (timer->due, t2 + 10);
		TEST_EQ (timer->timeout, 10);
		TEST_EQ_P (timer->callback, my_callback);
		TEST_EQ_P (timer->data, &timer);

		/* Check that the timer is the next one due. */
		TEST_EQ_P (nih_timer_next_due (), timer);

		nih_free (timer);
	}
}

void
test_add_periodic (void)
{
	NihTimer *timer;
	time_t    t1, t2;

	/* Check that we can add a periodic function and that the structure
	 * returned is correctly populated and placed in the timers list.
	 */
	TEST_FUNCTION ("nih_timer_add_periodic");
	nih_timer_poll ();
	TEST_ALLOC_FAIL {
		t1 = time (NULL);
		timer = nih_timer_add_periodic (NULL, 25, my_callback, &timer);
		t2 = time (NULL);

		if (test_alloc_failed) {
			TEST_EQ_P (timer, NULL);
			continue;
		}

		TEST_ALLOC_SIZE (timer, sizeof (NihTimer));
		TEST_LIST_NOT_EMPTY (&timer->entry);
		TEST_EQ (timer->type, NIH_TIMER_PERIODIC);
		TEST_GE (timer->due, t1 + 25);
		TEST_LE (timer->due, t2 + 25);
		TEST_EQ (timer->timeout, 25);
		TEST_EQ_P (timer->callback, my_callback);
		TEST_EQ_P (timer->data, &timer);

		/* Check that the timer is the next one due. */
		TEST_EQ_P (nih_timer_next_due (), timer);

		nih_free (timer);
	}
}

void
test_add_scheduled (void)
{
	NihTimer         *timer;
	NihTimerSchedule  schedule;
	time_t            t1, t2;

	/* Check that we can add a scheduled timer and that the structure
	 * returned is correctly populated, including copying the schedule
	 * from the structure we gave and placed in the timers list.
	 */
	TEST_FUNCTION ("nih_timer_add_scheduled");
	nih_timer_poll ();
	TEST_ALLOC_FAIL {
		memset (&schedule, 0, sizeof (NihTimerSchedule));

		t1 = time (NULL);
		timer = nih_timer_add_scheduled (NULL, &schedule,
						 my_callback, &timer);
		t2 = time (NULL);

		if (test_alloc_failed) {
			TEST_EQ_P (timer, NULL);
			continue;
		}

		TEST_ALLOC_SIZE (timer, sizeof (NihTimer));
		TEST_LIST_NOT_EMPTY (&timer->entry);
		TEST_EQ (timer->type, NIH_TIMER_SCHEDULED);
		TEST_EQ (timer->schedule.minutes, schedule.minutes);
		TEST_EQ (timer->schedule.hours, schedule.hours);
		TEST_EQ (timer->schedule.mdays, schedule.mdays);
		TEST_EQ (timer->schedule.months, schedule.months);
		TEST_EQ (timer->schedule.wdays, schedule.wdays);
		TEST_EQ_P (timer->callback, my_callback);
		TEST_EQ_P (timer->data, &timer);

		/* Check that the timer is the next one due. */
		TEST_EQ_P (nih_timer_next_due (), timer);

		nih_free (timer);
	}
}


void
test_next_due (void)
{
	NihTimer *timer1, *timer2, *timer3;

	/* Check that timers become due in the correct order by scheduling
	 * three in a random order, and then iterating through until there
	 * are no more left.
	 */
	TEST_FUNCTION ("nih_timer_next_due");
	timer1 = nih_timer_add_timeout (NULL, 10, my_callback, &timer1);
	timer2 = nih_timer_add_timeout (NULL, 5, my_callback, &timer2);
	timer3 = nih_timer_add_timeout (NULL, 15, my_callback, &timer3);

	TEST_EQ_P (nih_timer_next_due (), timer2);
	nih_free (timer2);

	TEST_EQ_P (nih_timer_next_due (), timer1);
	nih_free (timer1);

	TEST_EQ_P (nih_timer_next_due (), timer3);
	nih_free (timer3);

	TEST_EQ_P (nih_timer_next_due (), NULL);
}


void
test_poll (void)
{
	NihTimer *timer1, *timer2;
	time_t    t1, t2;

	TEST_FUNCTION ("nih_timer_poll");
	timer1 = nih_timer_add_timeout (NULL, 10, my_callback, &timer1);
	timer2 = nih_timer_add_periodic (NULL, 20, my_callback, &timer2);

	TEST_FREE_TAG (timer1);
	TEST_FREE_TAG (timer2);

	/* Check that we can poll for timers to be triggered, and have the
	 * first timeout run.  Once run, the timer should be destroyed.
	 */
	TEST_FEATURE ("with timeout");
	callback_called = 0;
	last_data = NULL;
	last_timer = NULL;

	timer1->due = time (NULL) - 5;
	nih_timer_poll ();

	TEST_EQ (callback_called, 1);
	TEST_EQ_P (last_timer, timer1);
	TEST_EQ_P (last_data, &timer1);

	TEST_FREE (timer1);


	/* Check that we can poll again and have the periodic timer run,
	 * that should be rescheduled instead of destroyed.
	 */
	TEST_FEATURE ("with periodic timer");
	callback_called = 0;
	last_data = NULL;
	last_timer = NULL;

	timer2->due = time (NULL) - 5;
	t1 = time (NULL);
	nih_timer_poll ();
	t2 = time (NULL);

	TEST_EQ (callback_called, 1);
	TEST_EQ_P (last_timer, timer2);
	TEST_EQ_P (last_data, &timer2);
	TEST_NOT_FREE (timer2);
	TEST_GE (timer2->due, t1 + 20);
	TEST_LE (timer2->due, t2 + 20);


	nih_free (timer2);
}


int
main (int   argc,
      char *argv[])
{
	test_add_timeout ();
	test_add_periodic ();
	test_add_scheduled ();
	test_next_due ();
	test_poll ();

	return 0;
}
