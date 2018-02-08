#include <iostream>
#include <memory>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <chrono>

#include <boost/smart_ptr/detail/spinlock_sync.hpp>
#include <stdint.h>
#include <boost/smart_ptr.hpp>

#include <auto_ptr.h>


using namespace std;
using namespace boost;

atomic_int a_var(0);
int v_=0;

class test{
public:
    int a;
    char b;
    test(){
        a=1;
        b=65;
    }
    void print(){
        std::cout<<a<<b<<std::endl;
    }
};

void weak_ptr_test(){
    test * atest = new test();
    boost::shared_ptr<test> ptest(atest);
//    boost::scoped_ptr<test> psc(atest);
    boost::weak_ptr<test> pweak(ptest);
    std::cout<<"shared_ptr "<<ptest.use_count()<<", weak_ptr "<<pweak.use_count()<<std::endl;
}

void * thread_func1(void * argv){
    while(1){
        int t_v=0;
        a_var+=2; //原子乘法运算
        std::cout<<"atomic op +2:"<<a_var<<std::endl;
        usleep(1000);
        t_v = __sync_fetch_and_add(&v_, 1);
        std::cout<<"lock:"<<v_<<std::endl;
    }
    return argv;
}

void * thread_func2(void * argv){
    while(1){
        a_var+=1; //原子加法运算
        std::cout<<"atomic op +1:"<<a_var<<std::endl;
        usleep(1000);
    }
    return argv;
}

int main(){
//    pthread_t threads1, threads2;
//    pthread_create(&threads1,NULL,thread_func1, NULL);
//    pthread_create(&threads2,NULL,thread_func2, NULL);
//    pthread_exit(NULL);

    weak_ptr_test();

    return 0;
}