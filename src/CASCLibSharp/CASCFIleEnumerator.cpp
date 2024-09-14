#include "pch.h"
#include "InteropUtility.h"
#include "CASCStorage.h"
#include "CASCFileInfo.h"
#include <vector>

using namespace CASCLibSharp;

CASCLibSharp::CASCEnumerator::CASCEnumerator(array<T>^ d)
{

};

IEnumerator<T>^ CASCLibSharp::CASCEnumerator::GetEnumerator()
{
}

CASCLibSharp::CASCEnumerator::~CASCEnumerator()
{
}

/*
public ref struct CASCEnumerator : public IEnumerable<T>
{

    CASCEnumerator(array<T>^ d)
    {
        data = d;
    }

    ref struct enumerator : IEnumerator<T>
    {
        enumerator(CASCEnumerator^ myArr)
        {
            colInst = myArr;
            currentIndex = -1;
        }

        virtual bool MoveNext() = IEnumerator<T>::MoveNext
        {
            if (currentIndex < colInst->data->Length - 1)
            {
                currentIndex++;
                return true;
            }
            return false;
        }

        property T Current
        {
            virtual T get() = IEnumerator<T>::Current::get
            {
                return colInst->data[currentIndex];
            }
        };

        // This is required as IEnumerator<T> also implements IEnumerator
        property Object^ Current2
        {
            virtual Object^ get() = System::Collections::IEnumerator::Current::get
            {
                return colInst->data[currentIndex];
            }
        };

        virtual void Reset() = IEnumerator<T>::Reset{}
            ~enumerator() {}

        CASCEnumerator^ colInst;
        int currentIndex;
    };

    array<T>^ data;

    virtual IEnumerator<T>^ GetEnumerator()
    {
        return gcnew enumerator(this);
    }

    virtual System::Collections::IEnumerator^ GetEnumerator2() = System::Collections::IEnumerable::GetEnumerator
    {
        return gcnew enumerator(this);
    }
};
*/
