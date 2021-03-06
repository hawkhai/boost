#ifndef SHARED_MUTEX_LOCKING_THREAD_HPP
#define SHARED_MUTEX_LOCKING_THREAD_HPP

//  (C) Copyright 2008 Anthony Williams
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/shared_mutex.hpp>

template<typename lock_type>
class locking_thread
{
    boost::shared_mutex& rw_mutex;
    unsigned& unblocked_count;
    boost::condition_variable& unblocked_condition;
    unsigned& simultaneous_running_count;
    unsigned& max_simultaneous_running;
    boost::mutex& unblocked_count_mutex;
    boost::mutex& finish_mutex;
public:
    locking_thread(boost::shared_mutex& rw_mutex_,
                   unsigned& unblocked_count_,
                   boost::mutex& unblocked_count_mutex_,
                   boost::condition_variable& unblocked_condition_,
                   boost::mutex& finish_mutex_,
                   unsigned& simultaneous_running_count_,
                   unsigned& max_simultaneous_running_):
        rw_mutex(rw_mutex_),
        unblocked_count(unblocked_count_),
        unblocked_condition(unblocked_condition_),
        simultaneous_running_count(simultaneous_running_count_),
        max_simultaneous_running(max_simultaneous_running_),
        unblocked_count_mutex(unblocked_count_mutex_),
        finish_mutex(finish_mutex_)
    {}
        
    void operator()()
    {
        // acquire lock
        lock_type lock(rw_mutex);
            
        // increment count to show we're unblocked
        {
            boost::mutex::scoped_lock ublock(unblocked_count_mutex);
            ++unblocked_count;
            unblocked_condition.notify_one();
            ++simultaneous_running_count;
            if(simultaneous_running_count>max_simultaneous_running)
            {
                max_simultaneous_running=simultaneous_running_count;
            }
        }
            
        // wait to finish
        boost::mutex::scoped_lock finish_lock(finish_mutex);
        {
            boost::mutex::scoped_lock ublock(unblocked_count_mutex);
            --simultaneous_running_count;
        }
    }
private:
    void operator=(locking_thread&);
};


#endif
