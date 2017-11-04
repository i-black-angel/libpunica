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
#include <punica/pfileinfo.h>

PUNICA_BEGIN_NAMESPACE

static bool __access(const std::string &__name, int __type)
{
	int err = 0;
	if (__name.empty()) {
		// log_error("filename is empty");
		return false;
	}
	if ((err = access(__name.c_str(), __type)) != 0) {
		// errno
		// log_error(strerror(errno));
		return false;
	}
	return true;
}

static std::string slash()
{
#ifdef P_OS_WIN
	return "/\\";
#else
	return "/";
#endif /* P_OS_WIN */
}

PFileInfo::PFileInfo()
{
}

PFileInfo::PFileInfo(const std::string &file)
{
	setFile(file);
}

// PFileInfo(const PFile &file);
// PFileInfo(const PDir &dir, const std::string &file);

PFileInfo::PFileInfo(const PFileInfo &fileinfo)
{
	_file = fileinfo._file;
}

PFileInfo::~PFileInfo()
{
}

PFileInfo& PFileInfo::operator=(const PFileInfo &fileinfo)
{
	_file = fileinfo._file;
	return *this;
}

bool PFileInfo::operator==(const PFileInfo &fileinfo) const
{
	return _file == fileinfo._file;
}

void PFileInfo::setFile(const std::string &file)
{
	_file = file;
}

// void setFile(const PFile &file);
// void setFile(const PDir &dir, const std::string &file);

bool PFileInfo::exists() const
{
	return exists(_file);
}

bool PFileInfo::exists(const std::string &file)
{
	return __access(file, F_OK);
}

// void PFileInfo::refresh()
// {
// }

std::string PFileInfo::filePath() const
{
	return dirname();
}

std::string PFileInfo::absoluteFilePath() const
{
}

std::string PFileInfo::canonicalFilePath() const
{
}

std::string PFileInfo::filename() const
{
	std::string file = basename();
	size_t idx = file.find_last_of(".");
	if (idx == std::string::npos) { return file; }
    return file.substr(0, idx);	
}

std::string PFileInfo::basename() const
{
	std::string file = _file;
	size_t idx = file.find_last_of(slash().c_str());
	if (idx == std::string::npos) { return file; }
	return file.substr(idx + 1);
}

std::string PFileInfo::completeBaseName() const
{
}

std::string PFileInfo::dirname() const
{
	std::string file = _file;
	size_t idx = file.find_last_of(slash().c_str());
	if (idx == std::string::npos) { return std::string("."); }
	if (idx == 0) idx = 1;
    return file.substr(0, idx);
}

std::string PFileInfo::suffix() const
{
	std::string file = basename();
	size_t idx = file.find_last_of(".");
	if (idx == std::string::npos) { return std::string(""); }
    return file.substr(idx + 1);
}

std::string PFileInfo::bundleName() const
{
}

std::string PFileInfo::completeSuffix() const
{
}


std::string PFileInfo::path() const
{
	return _file;
}

std::string PFileInfo::absolutePath() const
{
}

std::string PFileInfo::canonicalPath() const
{
}

// Dir dir() const
// Dir absoluteDir() const

bool PFileInfo::isReadable() const
{
	return __access(_file.c_str(), R_OK | F_OK);
}

bool PFileInfo::isWritable() const
{
	return __access(_file.c_str(), W_OK | F_OK);
}

bool PFileInfo::isExecutable() const
{
	return __access(_file.c_str(), X_OK | F_OK);
}

bool PFileInfo::isHidden() const
{
}

bool PFileInfo::isNativePath() const
{
}

bool PFileInfo::isRelative() const
{
}

bool PFileInfo::makeAbsolute()
{
}

bool PFileInfo::isFile() const
{
}

bool PFileInfo::isDir() const
{
}

bool PFileInfo::isSymLink() const
{
}

bool PFileInfo::isRoot() const
{
}

bool PFileInfo::isBundle() const
{
}

std::string PFileInfo::readLink() const
{
	struct stat sb;
	ssize_t r = 0;
	char *linkname = NULL;

	if (lstat(_file.c_str(), &sb) == -1) {
		perror("lstat");
		return std::string();
	}
	linkname = (char *) ::malloc(sb.st_size + 1);
	if (linkname == NULL) {
		fprintf(stderr, "insufficient memory\n");
		return std::string();
	}

	r = ::readlink(_file.c_str(), linkname, sb.st_size + 1);

	if (r == -1) {
		perror("lstat");
		return std::string();
	}

	if (r > sb.st_size) {
		fprintf(stderr, "symlink increased in size "
			"between lstat() and readlink()\n");
		return std::string();
	}

	linkname[r] = '\0';

	std::string res = linkname;
	::free(linkname);
	
	return res;
}

std::string PFileInfo::owner() const
{
}

uint32_t PFileInfo::ownerId() const
{
}

std::string PFileInfo::group() const
{
}

uint32_t PFileInfo::groupId() const
{
}

// bool permission(PFile::Permissions permissions) const
// PFile::Permissions permissions() const

int64_t PFileInfo::size() const
{
}

PDateTime PFileInfo::created() const
{
}

PDateTime PFileInfo::lastModified() const
{
}

PDateTime PFileInfo::lastRead() const
{
}

PUNICA_END_NAMESPACE
