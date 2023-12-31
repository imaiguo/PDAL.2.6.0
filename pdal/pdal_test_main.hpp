/******************************************************************************
* Copyright (c) 2015,  Hobu Inc. (info@hobu.co)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. nor the names of its
*       contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/
#include <locale>

#include "gtest/gtest.h"

// NOTE:  This file contains main() for all tests, and therefore 
//   should be included only once for each test executable.  Usually test
//   executables consist of a single source file, but if this isn't the case
//   you may need to take precautions or define your own main().

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    // Set the locale if requested for the tests,
    //  from an envirtonment variable, or test argument
    const char* envLocale = std::getenv("TEST_SET_LOCALE"); // for instance "de_DE.utf8"
    std::string localeStr = envLocale ? envLocale : "";
    const std::string prefix{"--set-locale="};
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]).rfind(prefix) == 0)
            localeStr = std::string(argv[i]).substr(prefix.size());
    }
    if (!localeStr.empty())
    {
        try
        {
            std::locale::global(std::locale(localeStr));
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << "Cannot set locale " << localeStr
                      << " . Probably not installed \n" << e.what() << '\n';
            return 1;
        }
    }
    return RUN_ALL_TESTS();
}

