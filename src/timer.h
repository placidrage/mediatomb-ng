/*  timer.h - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "zmm/zmm.h"
#include "zmmf/zmmf.h"
#include "sync.h"


// greatest common divisor for the signal times
// (in seconds)
#define TIMER_GCD   1

class TimerSubscriber : public zmm::Object
{
public:
virtual ~TimerSubscriber() { }
    virtual void timerNotify() = 0;
    
protected:
    friend class Timer;
};

class Timer : public zmm::Object
{
public:
    Timer();
    ~Timer();
    static zmm::Ref<Timer> getInstance();
    
    void addTimerSubscriber(zmm::Ref<TimerSubscriber> timerSubscriber, unsigned int notifyInterval);
    
    inline pthread_cond_t *getCond() { return &cond; }
    
    void triggerWait();
    
protected:
    
    class TimerSubscriberElement : public zmm::Object
    {
    public:
        TimerSubscriberElement(zmm::Ref<TimerSubscriber> subscriber, unsigned int notifyInterval);
        inline unsigned int getNotifyInterval() { return notifyInterval; }
        inline zmm::Ref<TimerSubscriber> getSubscriber() { return subscriber; }
        inline void notified();
        inline struct timespec *getNextNotify() { return &nextNotify; }
    protected:
        zmm::Ref<TimerSubscriber> subscriber;
        unsigned int notifyInterval;
        struct timespec nextNotify;
    };
    
    
    static zmm::Ref<Timer> instance;
    static Mutex mutex;
    pthread_cond_t cond;
    
    zmm::Ref<zmm::Array<TimerSubscriberElement> > subscribers;
    
    void notify();
    
    struct timespec *getNextNotifyTime();
};

#endif // __TIMER_H__
