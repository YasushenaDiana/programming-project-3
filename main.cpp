//Yasushena Diana К-28
#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <latch>
#include <syncstream>
#include <cstdlib>

using namespace std;

latch a_done(6);
latch b_done(9);
latch c_done(8);
latch d_done(7);
latch e_done(5);
latch f_done(6);
latch g_done(8);
latch h_done(6);
latch i_done(6);
latch j_done(8);

void f(char x, int i)
{
    this_thread::sleep_for(1s);
    osyncstream(cout) << "З набору " << x << " виконано дію " << i << "." << endl;
}

void run(char x, int from, int to, latch &done)
{
    for (int i = from; i <= to; ++i)
    {
        f(x, i);
        done.count_down();
    }
}

void run(char x, int from, int to)
{
    for (int i = from; i <= to; ++i)
        f(x, i);
}

void thread1()
{
    run('a', 1, 2, a_done);
    a_done.wait();
    run('c', 1, 2, c_done);
    run('d', 1, 1, d_done);
    run('b', 1, 2, b_done);
    d_done.wait();
    run('f', 3, 3, f_done);
    b_done.wait();
    run('e', 1, 1, e_done);
    c_done.wait();
    run('g', 1, 6, g_done);
    e_done.wait();
    g_done.wait();
    run('k', 1, 2);
}

void thread2()
{
    run('a', 3, 3, a_done);
    a_done.wait();
    run('c', 3, 4, c_done);
    run('d', 2, 2, d_done);
    run('b', 3, 4, b_done);
    d_done.wait();
    run('f', 4, 4, f_done);
    b_done.wait();
    run('e', 2, 2, e_done);
    c_done.wait();
    run('g', 7, 8, g_done);
    run('j', 1, 4, j_done);
    e_done.wait();
    g_done.wait();
    run('k', 3, 4);
}

void thread3()
{
    run('a', 4, 4, a_done);
    a_done.wait();
    run('c', 5, 6, c_done);
    run('d', 3, 3, d_done);
    run('b', 5, 6, b_done);
    d_done.wait();
    run('f', 5, 5, f_done);
    b_done.wait();
    run('e', 3, 3, e_done);
    c_done.wait();
    run('j', 5, 8, j_done);
    run('h', 1, 2, h_done);
    j_done.wait();
    h_done.wait();
    i_done.wait();
    run('n', 1, 2);
}

void thread4()
{
    run('a', 5, 5, a_done);
    a_done.wait();
    run('c', 7, 7, c_done);
    run('d', 4, 5, d_done);
    run('b', 7, 8, b_done);
    d_done.wait();
    run('f', 6, 6, f_done);
    b_done.wait();
    run('e', 4, 4, e_done);
    c_done.wait();
    run('h', 3, 6, h_done);
    run('i', 1, 2, i_done);
    j_done.wait();
    h_done.wait();
    i_done.wait();
    run('n', 3, 3);
    f_done.wait();
    run('m', 4, 4);
}

void thread5()
{
    run('a', 6, 6, a_done);
    a_done.wait();
    run('c', 8, 8, c_done);
    run('d', 6, 7, d_done);
    run('b', 9, 9, b_done);
    d_done.wait();
    run('f', 1, 2, f_done);
    b_done.wait();
    run('e', 5, 5, e_done);
    run('i', 3, 6, i_done);
    f_done.wait();
    run('m', 1, 3);
}

int main()
{
#if defined(_MSVC_LANG)
    system("chcp 65001");
#endif
    osyncstream(cout) << "Обчислення розпочато." << endl;
    auto time1 = chrono::system_clock::now();
    {
        jthread t1(thread1);
        jthread t2(thread2);
        jthread t3(thread3);
        jthread t4(thread4);
        jthread t5(thread5);
    }
    auto time2 = chrono::system_clock::now();
    chrono::duration<double> tdiff = time2 - time1;
    osyncstream(cout) << format("Обчислення завершено. Тривалість обчислення {:.2f} секунд.", tdiff.count()) << endl;
    return 0;
}
