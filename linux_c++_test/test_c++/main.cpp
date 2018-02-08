#include <iostream>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <new>
#include <typeinfo>
#include <vector>
#include <initializer_list>
#include <csignal>
#include <stdexcept>
#include <utility>
#include <tuple>
#include <set>

#ifdef WIN32
# aaaaa
#endif
#ifdef __linux
#include <unistd.h>
#endif // __linux

using namespace std;

class new_test;
void func(nullptr_t ptr);
void no_memory ();
void alloc_test();
void test_typeinfo();
void sighandler(int);
template<class _T1, class _T2>
inline void func2(_T1 _t1, _T2 _t2);
void tuple_test();

class new_test{
public:
    new_test(int i){
        std::cout<<"construct new_test: "<<i<<std::endl;
    }
    new_test(const std::initializer_list<int> &v ){
        for(std::initializer_list<int>::const_iterator it = v.begin();it!=v.end();it++){
            vec.push_back(*it);
        }
        for(std::vector<int>::const_iterator it=vec.begin();it!=vec.end();it++)
             std::cout<<*it<<std::endl;
    }
    new_test(){
        std::cout<<"construct new_test"<<std::endl;
    }
    ~new_test(){
        std::cout<<"destruct new_test"<<std::endl;
    }
private:
    vector<int> vec;
};

struct S {
    int n;
    std::string s;
    float d;
    bool operator<(const S& rhs) const
    {
        // compares n to rhs.n,
        // then s to rhs.s,
        // then d to rhs.d
        return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
    }
};

class Polymorphic {virtual void Member(){}};

int main()
{
//    nullptr_t a;
//    char* pPath;
//    pPath = getenv ("PATH");
//    if (pPath!=NULL)
//        printf ("The current path is: %s\n",pPath);
//    std::cout<<__cplusplus<<std::endl;
//    func(a);
//    new_test *atest = (new_test*)operator new(sizeof(new_test),std::nothrow);
//    getchar();
//    delete atest;
//    alloc_test();
//    test_typeinfo();
//    new_test arr = {1,2,3,4};
//    signal(SIGTERM, sighandler);
//    raise(SIGTERM);
//    pair<int,char> a = pair<int,char>(1,'a');
//    pair<int,char> b = make_pair(2,'b');
//    std::cout<<std::get<1>(b)<<std::endl;
//    std::swap(a,b);
//    std::cout<<std::get<1>(b)<<std::endl;
//    func2(1,'a');
    tuple_test();

    getchar();
    return 0;
}


void tuple_test(){
    std::set<S> set_of_s; // S is LessThanComparable

    S value{42, "Test", 3.14};
    std::set<S>::iterator iter;
    bool inserted;

    // unpacks the return value of insert into iter and inserted
    std::tie(iter, inserted) = set_of_s.insert(value);
    set_of_s.insert(value);

    if (inserted)
        std::cout << "Value was inserted successfully\n";
}

template<class _T1, class _T2>
inline  void func2(_T1 _t1, _T2 _t2){
    std::cout<<typeid(_t1).name()<<" : "<<_t1<<std::endl;
    std::cout<<typeid(_t2).name()<<" : "<<_t2<<std::endl;
}

void sighandler(int signum)
{
   std::cout<<"捕获信号"<<signum<<"跳出...\n";
   exit(1);
}

void test_typeinfo(){
//    new_test atest;
//    std::cout<<typeid(nullptr).name()<<std::endl;
    try
    {
        Polymorphic * pb = nullptr;
        std::cout << typeid(*pb).name();
    }
    catch (std::bad_typeid& bt)
    {
        std::cerr << "bad_typeid caught: " << bt.what() << '\n';
    }
}

void alloc_test(){
    std::set_new_handler(no_memory);
    std::cout << "Attempting to allocate 1 GiB..."<<std::endl;
    char* p = new char [100000000000000000000000000000000000];
    std::cout << "Ok\n";
    delete[] p;
}

void no_memory () {
  std::cout << "Failed to allocate memory!\n";
  std::exit (1);
}

void func(nullptr_t ptr){
    if(ptr==nullptr){
        std::cout<<"OK"<<std::endl;
    }
    else
        std::cout<<"NO"<<std::endl;
}
