// RightValue.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
//#include <windows.h>
using namespace std;

class CMyObj{
protected:
        unsigned m_iBufferSize;
        char* m_pBuffer;
public:
        CMyObj():m_iBufferSize(0),m_pBuffer(NULL){}
        CMyObj(unsigned int iBufSize):
                m_iBufferSize(iBufSize)
                {
                        m_pBuffer = new char[iBufSize];
                        memset(m_pBuffer,0,m_iBufferSize);
                }
        CMyObj(const CMyObj& objSrc){
                m_iBufferSize = objSrc.m_iBufferSize;
                if(m_iBufferSize > 0){
                        m_pBuffer = new char[m_iBufferSize];
                        memcpy(m_pBuffer,objSrc.m_pBuffer,m_iBufferSize);
                }
        }
        ~CMyObj(){
                if(m_pBuffer) delete m_pBuffer;
                m_pBuffer = NULL;
        }
};

class CMyObjWithMoveConstructor:public CMyObj
{
public:
        CMyObjWithMoveConstructor():CMyObj(){}
        CMyObjWithMoveConstructor(int iBufSize)
                :CMyObj(iBufSize){}
        //copy constructor
        CMyObjWithMoveConstructor(const CMyObjWithMoveConstructor& myObj)
                :CMyObj(myObj){}

        //move constructor
        CMyObjWithMoveConstructor(CMyObjWithMoveConstructor&& myObj){
                m_iBufferSize = myObj.m_iBufferSize;
                m_pBuffer = myObj.m_pBuffer;

                myObj.m_pBuffer = NULL;
                myObj.m_iBufferSize = 0;
        }

};

int main()
{
        static int iBufSize = 1024*1024*2; //2M
        static int iObjCnt = 100;

        clock_t t1,t2;
        double times;

        vector<CMyObj> vec;
        t1 = clock();
        // Not the right references
//        for( int i = 0; i < iObjCnt; i ++){ // Join the 100 elements
//                vec.push_back( CMyObj(iBufSize));
//        }
        t2 = clock();
        times = (double)(t2-t1)/CLOCKS_PER_SEC;
        cout<<"without move constructor:"<<times<<endl;

        vector<CMyObjWithMoveConstructor> vec2;
        t1 = clock();
        // Right references
        for( int j = 0; j < iObjCnt; j ++){ // Join the 100 elements
                vec2.push_back( CMyObjWithMoveConstructor(iBufSize));
        }
        t2 = clock();
        times = (double)(t2-t1)/CLOCKS_PER_SEC;
        cout<<"with move constructor:"<<times<<endl;

        return 0;
}

// The final results :
//without move constructor:1.48
//with move constructor:0.16
// Difference between 9.25 times times

