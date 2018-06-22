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
#include <minion/mlog.h>
#include <minion/msysinfo.h>
#include <minion/mdatetime.h>
#include <minion/mapplication.h>
#include <minion/mstring.h>
#include <minion/merror.h>

MINION_BEGIN_NAMESPACE

MLog *MLog::_ins = NULL;

MLog *MLog::instance()
{
	if (NULL == _ins) {
		_ins = new MLog();
		atexit(desposed);
	}
	return _ins;
}

void MLog::desposed()
{
	if (NULL != _ins) {
		delete _ins; _ins = NULL;
	}
}

MLog::MLog()
{
	_priority = kDebug;
	_performance = kNormal;
}

MLog::~MLog()
{
}

void MLog::init(int pri, const char *logfile, int performance)
{
	_priority = pri;
	if (logfile != NULL) {
		_logfile = logfile;
	}
	_performance = performance;
}

void MLog::log(const std::string &file, const std::string &func,
			   uint32_t line, int pri,
			   const char *__format, ...)
{
	MScopedLock locker(_mutex);
	
	// according to pri
	if (pri > _priority) return;
	
	// combine log head
	std::string head;

	int len;
	std::string buffer;
	va_list vargs;
	va_start(vargs, __format);
	len = vsnprintf(NULL, 0, __format, vargs);
	buffer.resize(len + 2);
	va_start(vargs, __format);
	vsnprintf(&buffer[0], len + 1, __format, vargs);
	va_end(vargs);

	// DATETIME HOSTNAME APPLICATIONNAME[PID] FILE FUNC[LINE] 
	std::string logstr = MString::format("%s %s %s[%lld] %s %s[%u]: <%s> %s",
								now().c_str(), hostname().c_str(),
								applicationName().c_str(), pid(),
								file.c_str(), func.c_str(), line,
								strpriority(pri).c_str(),
								buffer.c_str());

	std::cout << logstr << std::endl;

	if (_logfile.empty()) {
		syslog(LOG_DEBUG, "%s", logstr.c_str());
	} else {
		FILE *fp = fopen(_logfile.c_str(), "a");
		if (fp == NULL) {
			std::string errstr = MString::format("open %s failed: %s",
												 _logfile.c_str(), error().c_str());
			fprintf(stderr, "%s\n", errstr.c_str());
			syslog(LOG_ERR, "%s", errstr.c_str());
			return;
		}
		fprintf(fp, "%s\n", logstr.c_str());
		fflush(fp);
		fclose(fp);
	}
}

std::string MLog::strpriority(int pri) const
{
	std::string ret = "debug";
	switch (pri) {
	case kEmerg:
		ret = "emerg"; break;
	case kAlert:
		ret = "alert"; break;
	case kCrit:
		ret = "crit"; break;
	case kError:
		ret = "error"; break;
	case kWarn:
		ret = "warn"; break;
	case kNotice:
		ret = "notice"; break;
	case kInfo:
		ret = "info"; break;
	default:
		break;
	}
	return ret;
}

MINION_END_NAMESPACE
