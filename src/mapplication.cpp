/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <minion/mapplication.h>

MINION_BEGIN_NAMESPACE

MApplication::MApplication(int argc, char *argv[])
{
}

MApplication::~MApplication()
{
}

std::string MApplication::applicationDirPath()
{
}

std::string MApplication::applicationFilePath()
{
}

std::string MApplication::applicationName()
{
}

std::string MApplication::applicationVersion()
{
}

int64_t MApplication::applicationPid()
{
    return getpid();
}

uint32_t MApplication::uptime()
{
}

void MApplication::setApplicationName(const std::string &application)
{
	_applicationName = application;
}

void MApplication::setApplicationVersion(const std::string &version)
{
	_applicationVersion = version;
}

bool MApplication::alreadyRunning(const std::string &lockfile)
{
}

MINION_END_NAMESPACE
