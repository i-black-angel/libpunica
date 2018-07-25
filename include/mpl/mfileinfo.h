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
#ifndef _MFILEINFO_H_
#define _MFILEINFO_H_

#include <mpl/mcoredef.h>
#include <mpl/mdatetime.h>

MPL_BEGIN_NAMESPACE

class MFileInfo
{
public:
    MFileInfo();
	MFileInfo(const std::string &file);
	// MFileInfo(const MFile &file);
	// MFileInfo(const MDir &dir, const std::string &file);
	MFileInfo(const MFileInfo &fileinfo);
    virtual ~MFileInfo();

	MFileInfo& operator=(const MFileInfo &fileinfo);
	bool operator==(const MFileInfo &fileinfo) const;
	inline bool operator!=(const MFileInfo &fileinfo) const { return !operator==(fileinfo); }

	void setFile(const std::string &file);
	// void setFile(const MFile &file);
	// void setFile(const MDir &dir, const std::string &file);
	bool exists() const;
	static bool exists(const std::string &file);
	// void refresh();

	std::string filePath() const;
	std::string absoluteFilePath() const;
	std::string canonicalFilePath() const;
	std::string dirname() const;
	std::string basename() const;
	std::string filename() const;
	std::string suffix() const;
	std::string completeBaseName() const;
	std::string bundleName() const;
	std::string completeSuffix() const;

	std::string path() const;
	std::string absolutePath() const;
	std::string canonicalPath() const;
	// Dir dir() const;
	// Dir absoluteDir() const;

	bool isReadable() const;
	bool isWritable() const;
	bool isExecutable() const;
	bool isHidden() const;
	bool isNativePath() const;

	bool isRelative() const;
	inline bool isAbsolute() const { return !isRelative(); }
	bool makeAbsolute();

	bool isFile() const;
	bool isDir() const;
	bool isSymLink() const;
	bool isRoot() const;
	bool isBundle() const;

	std::string readLink() const;
	inline std::string symLinkTarget() const { return readLink(); }

	std::string owner() const;
	uint32_t ownerId() const;
	std::string group() const;
	uint32_t groupId() const;

	// bool permission(MFile::Permissions permissions) const;
	// MFile::Permissions permissions() const;

	int64_t size() const;

	MDateTime created() const;
	MDateTime lastModified() const;
	MDateTime lastRead() const;
private:
	std::string _file;
};

MPL_END_NAMESPACE

#endif /* _MFILEINFO_H_ */