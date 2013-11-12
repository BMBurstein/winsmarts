/*!
 * \file WinSMARTS.h
 *
 * \author Baruch Burstein
 * \author Elad Yechiel
 *
 * Main header file for WinSMARTS library
 */
#pragma once
#ifndef WINSMARTS_H
#define WINSMARTS_H

#include "platform.h"
#include "Task.h"
#include "asm.h"
#include "schedAlgo.h"
#include "Log.h"
#include "timer.h"

#include <string>
#include <vector>

//! The type used for thread ids
typedef size_t tid_t;

/*!
 * \class WinSMARTS
 *
 * \brief The main class for multi-tasking
 *
 *
 * A single instance of this class may exist per system thread
 * This is not enforced by the class itself, but trying to use
 * multiple instances will likely crash. Bang, your dead.
 */
class WinSMARTS
{
private:
	typedef std::vector<std::shared_ptr<Task> > Tasks;  //!< The type of the list of tasks

	Tasks         tasks;              //!< The list of all tasks ever created
	unsigned int  timerInterval;      //!< Milliseconds between timer pulses
	tid_t         currentTask;        //!< Index of currently running task
	SchedAlgo*    algo;               //!< Pointer to the scheduler function
	bool          deadlock;           //!< Signals that a deadlock was detected in scheduler
	bool          contextSwitchAllow; //!< If false context switching is disabled
	bool          endOfTimeSlice;     //!< Indicates a timer interrupt occured while context switch was disabled
	bool          ranAll;             //!< Indicates that all tasks have completed
	TaskObj       myContext;          //!< Context of runTheTasks() (the scheduler)
	Log&          logger;             //!< Output log receiver
	volatile bool debug;              //!< Signals that debugging is requested
	volatile bool pause;              //!< Signals that the system is paused
	timerHandle   timer;              //!< Handle to the preemptive timer

	///@{
	/*!
	 * \brief Sends message to logger
	 * \param[in] type Message class
	 * \param[in] msg  String to log
	 * \sa Log, LogUDP, LogFile
	 */
	void log(LogMsg type, std::string const& msg);    //Log function for string
	/*!
	 * \brief Sends message to logger
	 * \param[in] type Message class
	 * \param[in] msg  Printf-style string
	 * \param[in] ...  Variables that will be expanded printf-style
	 * \sa Log, LogUDP, LogFile
	 */
	void log(LogMsg type, char const* msg = "", ...); //Log function for char*
	///@}

	/// Copy constructor private and unimplemented to prevent cloning
	WinSMARTS(WinSMARTS const&);
	/// Assignment operator private and unimplemented to prevent cloning
	WinSMARTS& operator=(WinSMARTS const&);

	/// Pauses system if debug mode was requested
	void breakForDebug();

	void setCurrentTask(tid_t tid)                 { currentTask = tid; }                      // Set tid to be run
	void sleepDecr(tid_t tid)                      { tasks.at(tid)->sleepDecr(); }             // Decrease task's sleep time

	friend Task;

public:
	/*!
	 * \brief WinSMARTS constructor
	 * \param[in] scheduler Pointer to schedueling function
	 * \param[in] logger    Instance of Log that all log output will be sent too
	 * \param[in] interval  Size, in miliseconds, of each time slice (approx.)
	 * \sa Log, SchedAlgo
	 */
	WinSMARTS(SchedAlgo* scheduler, Log& logger, unsigned int interval = 55);
	
	/// Starts running this WinSMARTS instance.
	/** Will not return until done running all tasks */
	void runTheTasks();                                                                  // Start running the tasks
	/*!
	 * \brief Adds a new task to the #tasks vector
	 * \param[in] fn       Function pointer where to start the task
	 * \param[in] name     Name of the task as will appear in the log
	 * \param[in] priority Initial priority of this task
	 */
	tid_t declareTask(TaskProc fn, std::string const &name, unsigned int priority);      // Add a new task to the tasks vector
	/// Saves the current context and switches context back to runTheTasks()
	void callScheduler() { tasks[getCurrentTask()]->switchTo(myContext); }               // Return the control to the scheduler

	// Task managment
	/*!
	 * \brief Puts current task to sleep for \a ms miliseconds (approx.) 
	 * \param[in] ms Time, in miliseconds, the task should sleep
	 * \sa Task::setSleep(), isTaskSleeping()
	 */
	void    sleep(unsigned int ms);                                                      // Send currnet task to sleep
	/*!
	 * \brief Allows context switch to occur
	 * \sa contextSwitchAllow, contextSwitchOff()
	 */
	void    contextSwitchOn();                                                           // Enable context switch
	/*!
	 * \brief Disables context switching
	 * \sa contextSwitchAllow, contextSwitchOn()
	 */
	void    contextSwitchOff();                                                          // Disable context switch
	//TaskRef getTasksByStatus(taskStatus stat);                                           // Get task list by task status
	
	
	/*!
	 * \brief Checks of there is a sleeping task
	 * \sa Task::setSleep(), sleep()
	 */
	bool isTaskSleeping();
	bool isMoreThanOneTaskAlive();

	// WinSMARTS accessors
	/// Gets task id of currently running task
	tid_t       getCurrentTask()           const { return currentTask; }                       // Tid of running task
	/// Gets total number of tasks, in any state
	size_t      getTotalTasks()            const { return tasks.size(); }                      // Number of declared tasks
	/// Checks if a deadlock occured
	bool        getDeadlock()              const { return deadlock; }                          // True if in deadlock
	/// Checks if context switch is allowed now
	bool        getContextSwitchAllow()         const { return contextSwitchAllow; }                // True if context switching allowed

	// Task accessors
	/*!
	 * \brief Gets the name of task \a tid
	 * \param[in] tid ID of task to get
	 */
	std::string getTaskName(tid_t tid)     const { return tasks.at(tid)->getName(); }          // Get task's name by it's tid
	/// Gets the name of the current task
	std::string getTaskName()              const { return getTaskName(getCurrentTask()); }     // Get current task's name
	/*!
	 * \brief Gets the status of task \a tid
	 * \param[in] tid ID of task to get
	 */
	taskStatus  getTaskStatus(tid_t tid)   const { return tasks.at(tid)->getStatus(); }        // Get task's status by it's tid
	/// Gets the status of the current task
	taskStatus  getTaskStatus()            const { return getTaskStatus(getCurrentTask()); }   // Get current task's status
	/*!
	 * \brief Gets the priority of task \a tid
	 * \param[in] tid ID of task to get
	 */
	int         getTaskPriority(tid_t tid) const { return tasks.at(tid)->getPriority(); }      // Get task's priority by it's tid
	/// Gets the priority of the current task
	int         getTaskPriority()          const { return getTaskPriority(getCurrentTask()); } // Get current task's priority

	// Task mutators
	/*!
	 * \brief Increases the priority of task \a tid
	 * \param[in] tid ID of task to set
	 */
	void incrTaskPriority(tid_t tid)               { tasks.at(tid)->incrPriority(); }          // Increase task's priority by tid
	/// Increases the priority of the current task
	void incrTaskPriority()                        { incrTaskPriority(getCurrentTask()); }     // Increase current task's priority
	/*!
	 * \brief Sets the status of task \a tid
	 * \param[in] tid  ID of task to set
	 * \param[in] stat the new status
	 */
	void setTaskStatus(tid_t tid, taskStatus stat) { tasks.at(tid)->setStatus(stat); }         // Set task's status by it's tid
	/*!
	 * \brief Sets the status of the current task
	 * \param[in] stat the new status
	 */
	void setTaskStatus(taskStatus stat)            { setTaskStatus(getCurrentTask(), stat); }  // Set current task's status
	/*!
	 * \brief Resets the priority of task \a tid back to it's original value
	 * \param[in] tid ID of task to set
	 */
	void restorePriority(tid_t tid)                { tasks.at(tid)->restorePriority(); }       // Restore task's priority by tid
	/// Resets the priority of the current task back to it's original value
	void restorePriority()                         { restorePriority(getCurrentTask()); }      // Restore current task's priority


	Event* getExpectedEvent(unsigned int tid){ return (tid >= 0 && tid <= getTotalTasks())? tasks.at(tid)->getExpectedEvent() : NULL; } //Get task's expectedEvent by it's index
	Event* getCurrentExpectedEvent() { return tasks.at(getCurrentTask())->getExpectedEvent(); }                                // Get current task's expectedEvent
	void setCurrentExpectedEvent(Event* expectedEventp) { tasks.at(getCurrentTask())->setExpectedEvent(expectedEventp); }      // Set current task's expectedEvent

	void taskEnd();
	void timerHandler();
	void systemIdle();

	// Debugging functions
	void debugBegin(); // this must be called before any debug functions!
	void debugEnd();   // call this when done debugging
	void debugStep();
	void debugSetCurrentTask(tid_t tid);
	void debugSetContextSwitch(bool allow);
};

#endif // WINSMARTS_H