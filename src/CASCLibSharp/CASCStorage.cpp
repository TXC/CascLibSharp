// ------------------------------------------------------------------------
// CASCLibNet - A wrapper around Ladislav Zezula's CASC Library for .NET
//
// Copyright(c) 2018 Philip/Scobalula
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ------------------------------------------------------------------------
// File: CASCStorage.cpp
// Author: Philip/Scobalula
// Description: This file contains the main CASC Storage Class
#include "pch.h"
#include "InteropUtility.h"
#include "CASCStorage.h"
#include "CASCFileInfo.h"
#include "CASCEnumerator.h"
#include <vector>

using namespace CASCLibSharp;

CASCLibSharp::CASCStorage::CASCStorage(String^ path, System::Int32 localeMask)
{
    // Convert to Native String
#ifdef _UNICODE
    std::wstring filePathStd;
    InteropUtility::ToStdWString(path, filePathStd);
#else
    std::string filePathStd;
    InteropUtility::ToStdString(path, filePathStd);
#endif

    _FilePath = path;

    HANDLE handle;
    size_t fileCount;
    size_t result;
    DWORD dwErrCode;

    if (!CascOpenStorage(filePathStd.c_str(), localeMask, &handle))
    {
        dwErrCode = GetLastError();
        throw gcnew System::IO::IOException("Failed to open CASC Storage", dwErrCode);
    }

    if(!CascGetStorageInfo(handle, CascStorageTotalFileCount, &fileCount, sizeof(size_t), &result))
    {
        dwErrCode = GetLastError();
        throw gcnew System::IO::IOException("Failed to get CASC Storage Info", dwErrCode);
    }

    _StorageHandle = handle;

    //_Files = gcnew array<CASCFileInfo^>((int)fileCount);

    _Files = Filter("*");
}

CASCLibSharp::CASCStorage::CASCStorage(String^ path) : CASCStorage(path, 0)
{
}

array<CASCFileInfo^>^ CASCLibSharp::CASCStorage::Filter(String^ filemask)
{
    // Convert to Native String
#ifdef _UNICODE
    std::wstring fileMaskhStd;
    InteropUtility::ToStdWString(filemask, fileMaskStd);
#else
    std::string fileMaskStd;
    InteropUtility::ToStdString(filemask, fileMaskStd);
#endif

    //size_t index = 0;
    HANDLE fileHandle;
    CASC_FIND_DATA findData;
    DWORD dwErrCode;

    fileHandle = CascFindFirstFile(_StorageHandle, fileMaskStd.c_str(), &findData, NULL);

    if (fileHandle == NULL)
    {
        dwErrCode = GetLastError();
        throw gcnew System::IO::IOException("Unable to find matches in CASC Storage", dwErrCode);
    }

    std::vector<CASC_FIND_DATA> result;
    do
    {
        result.push_back(findData);
    } while (CascFindNextFile(fileHandle, &findData));

    CascFindClose(fileHandle);

    int resultSize = (int)result.size();
    array<CASCFileInfo^>^ arr = gcnew array<CASCFileInfo^>(resultSize);

    if (resultSize == 0) {
        return arr;
    }

    for (int i = 0; i < resultSize; i++)
    {
        findData = result[i];

        auto cKey = gcnew array<System::Byte>(sizeof(findData.CKey));
        auto eKey = gcnew array<System::Byte>(sizeof(findData.EKey));

        Marshal::Copy(IntPtr(&findData.CKey), cKey, 0, cKey->Length);
        Marshal::Copy(IntPtr(&findData.EKey), eKey, 0, eKey->Length);

        CASCFileInfo^ file = gcnew CASCFileInfo(
            gcnew String(findData.szFileName),
            gcnew String(findData.szPlainName),
            findData.FileSize,
            findData.bFileAvailable == 1,
            cKey,
            eKey);

        arr[i] = file;
    }

    return arr;
}

CASCLibSharp::CASCFileStream^ CASCLibSharp::CASCStorage::OpenFile(String^ fileName)
{
    HANDLE handle;

    // Convert to Native String
#ifdef _UNICODE
    std::wstring filePathStd;
    InteropUtility::ToStdWString(fileName, filePathStd);
#else
    std::string filePathStd;
    InteropUtility::ToStdString(fileName, filePathStd);
#endif

    if (!CascOpenFile(_StorageHandle, filePathStd.c_str(), 0, 0, &handle))
        throw gcnew System::IO::IOException("Failed to open CASC File");

    return gcnew CASCLibSharp::CASCFileStream(IntPtr(handle));
}

CASCLibSharp::CASCStorage::~CASCStorage()
{
    _Files = nullptr;
    _Files = gcnew array<CASCFileInfo^>(0);
    CascCloseStorage(_StorageHandle);
    _StorageHandle = nullptr;
}
