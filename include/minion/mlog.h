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
#ifndef _MLOG_H_
#define _MLOG_H_

#include <minion/mcoredef.h>
#include <minion/mmutex.h>

MINION_BEGIN_NAMESPACE

class MLog
{
public:
	static MLog *instance();
	virtual ~MLog();
public:
	enum Priority {
		kEmerg = 0,
		kAlert,
		kCrit,
		kError,
		kWarn,
		kNotice,
		kInfo,
		kDebug
	};

	enum Performance {
		kNormal = 0,
		kHigh
	};

	void init(int priority = minion::MLog::kDebug,
			  const char *logfile = NULL,
			  int performance = minion::MLog::kNormal);
	void log(const std::string &srcfile, const std::string &srcfunc,
			  uint32_t srcline, int priority,
			  const char *__format, ...);
	std::string strpriority(int pri) const;
protected:
	MLog();
	static void desposed();
private:
	std::string _logfile;
	int _priority;
	int _performance;

	MMutex _mutex;
	
	static MLog *_ins;
};

MINION_END_NAMESPACE

#define log_init                  minion::MLog::instance()->init
#define log_emerg(__format, ...)  minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kEmerg, __format, ## __VA_ARGS__)
#define log_alert(__format, ...)  minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kAlert, __format, ## __VA_ARGS__)
#define log_crit(__format, ...)   minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kCrit, __format, ## __VA_ARGS__)
#define log_error(__format, ...)  minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kError, __format, ## __VA_ARGS__)
#define log_warn(__format, ...)   minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kWarn, __format, ## __VA_ARGS__)
#define log_notice(__format, ...) minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kNotice, __format, ## __VA_ARGS__)
#define log_info(__format, ...)   minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kInfo, __format, ## __VA_ARGS__)
#define log_debug(__format, ...)  minion::MLog::instance()->log(__FILE__, __func__, __LINE__, \
																 minion::MLog::kDebug, __format, ## __VA_ARGS__)

#endif /* _MLOG_H_ */
