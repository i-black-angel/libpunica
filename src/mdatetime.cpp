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
#include <mpl/mdatetime.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

static const char *shorWeekDay[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static const char *longWeekDay[] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static const char *shortMonth[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static const char *longMonth[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "Auguest", "September", "October", "November", "December"
};

std::string now()
{
	return MDateTime::now();
}

MTime::MTime()
{
	start();
}

MTime::~MTime()
{
}

void MTime::start()
{
#ifdef M_OS_WIN
	_start = clock();
#else
	gettimeofday(&_start, NULL);
#endif /* M_OS_LINUX */
}

double MTime::restart()
{
	double te = elapsed();
	// reset _START
	start();
	return te;
}

double MTime::elapsed() const
{
#ifdef M_OS_WIN
	double timeElapsed = 0;
	timeElapsed = (double)(clock() - _start) / CLOCKS_PER_SEC;
	return timeElapsed;
#else
	double timeElapsed = 0;
	struct timeval end;
	gettimeofday(&end, NULL);
	timeElapsed = (end.tv_sec - _start.tv_sec) * 1000000
		+ (end.tv_usec - _start.tv_usec);
	timeElapsed /= 1000000.0;
	return timeElapsed;
#endif /* M_OS_WIN */
}

MDateTime::MDateTime()
{
	_d = time(NULL);
	_valid = true;
}

MDateTime::MDateTime(time_t d)
{
	_d = d;
	_valid = true;
}

MDateTime::MDateTime(int y, int m, int d, int H, int M, int S)
{
	setDateTime(y, m, d, H, M, S);
	_valid = true;
}

MDateTime::MDateTime(const MDateTime &other)
{
	_d = other._d;
	_valid = other._valid;
}

MDateTime::MDateTime(const std::string &text)
{
	_valid = setDateTime(text);
}

MDateTime::MDateTime(const char *text)
{
	_valid = setDateTime(text);
}

MDateTime::~MDateTime()
{
}

MDateTime MDateTime::addDays(int days) const
{
	return _d + days * 86400;
}

MDateTime MDateTime::addSecs(int64_t s) const
{
	return _d + s;
}

int MDateTime::daysTo(const MDateTime &other) const
{
	time_t curMidDay = _d;
	struct tm *st = localtime(&curMidDay);
	st->tm_hour = 12;
	st->tm_min  = 0;
	st->tm_sec  = 0;
	curMidDay = mktime(st);
	
	time_t othMidDay = other.data();
	st = localtime(&othMidDay);
	st->tm_hour = 12;
	st->tm_min  = 0;
	st->tm_sec  = 0;
	othMidDay = mktime(st);
	
	return (othMidDay - curMidDay) / 86400;
}

int64_t MDateTime::secsTo(const MDateTime &other) const
{
	return other.data() - _d;
}

MDateTime &MDateTime::operator=(const MDateTime &other)
{
	_d = other._d;
	_valid = other._valid;
	return *this;
}

MDateTime &MDateTime::operator=(time_t val)
{
	_d = val;
	_valid = true;
	return *this;
}

MDateTime &MDateTime::operator=(const std::string &text)
{
	_valid = setDateTime(text);
	return *this;
}

MDateTime &MDateTime::operator=(const char *text)
{
	_valid = setDateTime(text);
	return *this;
}

int MDateTime::year() const
{
	return localtime(&_d)->tm_year + 1900;
}

int MDateTime::month() const
{
	return localtime(&_d)->tm_mon + 1;
}

int MDateTime::day() const
{
	return localtime(&_d)->tm_mday;
}

int MDateTime::hour() const
{
	return localtime(&_d)->tm_hour;
}

int MDateTime::minute() const
{
	return localtime(&_d)->tm_min;
}

int MDateTime::second() const
{
	return localtime(&_d)->tm_sec;
}

int MDateTime::dayOfWeek() const
{
	return localtime(&_d)->tm_wday;
}

int MDateTime::dayOfYear() const
{
	return localtime(&_d)->tm_yday;
}
	
std::string MDateTime::toString(const std::string &format) const
{
	std::string local_format = format;
	if (format.empty()) {
		local_format = "%Y-%m-%d %H:%M:%S";
	}
	char buffer[64] = {0};
	strftime(buffer, sizeof(buffer), local_format.c_str(), localtime(&_d));
	return std::string(buffer);
}

bool MDateTime::setDateTime(const std::string &text)
{
	// Accept formats is 'yyyymmdd', 'yyyymmddHHMMSS' or 'yyyy-mm-dd HH:MM:SS',
	// or 'yyyy-mm-dd'
	if (text.size() != 8 &&
		text.size() != 10 &&
		text.size() != 14 &&
		text.size() != 19) {
		return false;
	}

	// while format is 'yyyymmdd' or 'yyyymmddHHMMSS', make sure all
	// characters are numeric
	if (text.size() == 8 || text.size() == 14) {
		for (std::string::size_type i = 0; i < text.size(); ++i) {
			if (!isdigit(text[i])) {
				return false;
			}
		} // for

		switch (text.size()) {
		case 8:
			setDateTime(atoi(text.substr(0, 4).c_str()),
						atoi(text.substr(4, 2).c_str()),
						atoi(text.substr(6, 2).c_str()));
			break;
		case 14:
			setDateTime(atoi(text.substr(0, 4).c_str()),
						atoi(text.substr(4, 2).c_str()),
						atoi(text.substr(6, 2).c_str()),
						atoi(text.substr(8, 2).c_str()),
						atoi(text.substr(10, 2).c_str()),
						atoi(text.substr(12, 2).c_str()));
			break;
		}
		return true;
	} // fi

	// Analyse 'yyyy-mm-dd'
	if (text.size() == 10) {
		if (!isdigit(text[0]) ||
			!isdigit(text[1]) ||
			!isdigit(text[2]) ||
			!isdigit(text[3]) ||
			!isdigit(text[5]) ||
			!isdigit(text[6]) ||
			!isdigit(text[8]) ||
			!isdigit(text[9]))
			return false;

		if (text[4] != '-' || text[7] != '-')
			return false;

		setDateTime(atoi(text.substr(0, 4).c_str()),
					atoi(text.substr(5, 2).c_str()),
					atoi(text.substr(8, 2).c_str()));
		return true;
	}

	// Analyse 'yyyy-mm-dd HH:MM:SS'
	if (!isdigit(text[0]) ||
		!isdigit(text[1]) ||
		!isdigit(text[2]) ||
		!isdigit(text[3]) ||
		!isdigit(text[5]) ||
		!isdigit(text[6]) ||
		!isdigit(text[8]) ||
		!isdigit(text[9]) ||
		!isdigit(text[11]) ||
		!isdigit(text[12]) ||
		!isdigit(text[14]) ||
		!isdigit(text[15]) ||
		!isdigit(text[17]) ||
		!isdigit(text[18]))
		return false;

	if (text[4] != '-' || text[7] != '-')
		return false;

	if (text[10] != ' ')
		return false;
	
	if (text[13] != ':' || text[16] != ':')
		return false;

	setDateTime(atoi(text.substr(0, 4).c_str()),
				atoi(text.substr(5, 2).c_str()),
				atoi(text.substr(8, 2).c_str()),
				atoi(text.substr(11, 2).c_str()),
				atoi(text.substr(14, 2).c_str()),
				atoi(text.substr(17, 2).c_str()));
	return true;
}

void MDateTime::setDateTime(int y, int m, int d, int H, int M, int S)
{
	struct tm st;
	st.tm_year = y - 1900;
	st.tm_mon = m - 1;
	st.tm_mday = d;
	st.tm_hour = H;
	st.tm_min = M;
	st.tm_sec = S;

	_d = mktime(&st);
}

std::string MDateTime::now(const std::string &format)
{
	return currentDateTime().toString(format);
}

MDateTime MDateTime::currentDateTime()
{
	return time(NULL);
}

std::string MDateTime::longWeekdayName() const
{
	return longWeekdayName(dayOfWeek());
}

std::string MDateTime::longWeekdayName(int weekday)
{
	if (weekday < 0 || weekday > 6) return "";

	return longWeekDay[weekday];
}

std::string MDateTime::longMonthName() const
{
	return longMonthName(month());
}

std::string MDateTime::longMonthName(int month)
{
	if (month < 1 || month > 12) return "";

	return longMonth[month - 1];
}

std::string MDateTime::shortWeekdayName() const
{
	return shortWeekdayName(dayOfWeek());
}

std::string MDateTime::shortWeekdayName(int weekday)
{
	if (weekday < 0 || weekday > 6) return "";

	return shorWeekDay[weekday];
}

std::string MDateTime::shortMonthName() const
{
	return shortMonthName(month());
}

std::string MDateTime::shortMonthName(int month)
{
	if (month < 1 || month > 12) return "";

	return shortMonth[month - 1];
}

std::ostream &operator<<(std::ostream &out, const MDateTime &datetime)
{
	out << datetime.toString();
	return out;
}

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
