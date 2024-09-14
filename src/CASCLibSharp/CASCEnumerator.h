#pragma once
#include "pch.h"
#include "InteropUtility.h"

using namespace System;
using namespace System::Collections::Generic;

namespace CASCLibSharp
{
    /// <inheritdoc cref="IEnumerable"/>
    /// <typeparam name = "T" />
    generic <class T>
    public ref struct CASCEnumerator : public IEnumerable<T>
    {
    private:
        /// <summary>
        /// Hold data of type <typeparamref name="T"/>
        /// </summary>
        array<T>^ data;

        ref struct enumerator : IEnumerator<T>
        {
            enumerator(CASCEnumerator^ payload)
            {
                colInst = payload;
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

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="CASCEnumerator"/> class.
        /// </summary>
        /// <param name="arrayData">Array of data.</param>
        CASCEnumerator(array<T>^ arrayData)
        {
            data = arrayData;
        }

        /// <inheritdoc cref="GetEnumerator()"/>
        virtual IEnumerator<T>^ GetEnumerator()
        {
            return gcnew enumerator(this);
        }

        virtual System::Collections::IEnumerator^ GetEnumerator2() = System::Collections::IEnumerable::GetEnumerator
        {
            return gcnew enumerator(this);
        }
    };
}
