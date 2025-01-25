typedef void (*TaskFunction)(void *user_input);

#define MAX_TASKS 20

// Callback times for periodic tasks
#define CB_1MS      (0x02)             // Love for power of 2, MCU friendly
#define CB_10MS     (CB_1MS*10)
#define CB_100MS    (CB_10MS*10)
#define CB_500MS    (CB_100MS*5)
#define CB_1S       (CB_1MS*1000)
#define CB_1M       (CB_1S*60)
#define CB_1H       (CB_1M*60)
#define CB_1D       (CB_1H*24)

class Task
{
public:
    enum class Status
    {
        ACTIVE,
        PAUSED,
        KILLED
    };
    
    Task(int executionTime, int period, bool priority, TaskFunction func, void *user_input) : executionTime(executionTime), period(period), priority(priority), status(Status::ACTIVE), taskFunction(func), user_input(user_input){}
    void execute();
    void pause();
    void resume();
    void kill();
    Status getStatus() const;
    int getExecutionTime() const;
    bool getPriority() const;
    void setPriority(bool value);
    int getPeriod() const;
    unsigned long getNextExecutionTime() const;
    void setNextExecutionTime(unsigned long time);
    void *getUserInput() const;
private:
    void *user_input;
    int executionTime;
    bool priority;
    int period = CB_1S; // Default period is 1 second
    Status status = Status::ACTIVE; // Initialize status to ACTIVE
    TaskFunction taskFunction;
    unsigned long nextExecutionTime;
};