#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL\glew.h>
#include <Windows.h>

const wchar_t *GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

/* Returns a list of files in a directory (except the ones that begin with a dot) */
void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	 
	if ((dir = FindFirstFile(GetWC((directory + "/*").c_str()), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		char ch[260];
		char defChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, file_data.cFileName, -1, ch, 260, &defChar, NULL);
		const std::string file_name(ch);
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
} // GetFilesInDirectory

bool convertFile(const std::string &fileName)
{
	std::ifstream in("./obj/" + fileName, std::ios::in);
	std::vector<GLfloat> objData;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> uvs;
	std::vector<GLfloat> normals;

	if (!in)
	{
		std::cout << "Cannot open " << fileName << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			float x = 0.f;
			float y = 0.f;
			float z = 0.f;

			if (sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z) < 3)
			{
				std::cout << "ERROR: OBJ formatting error: " << line << std::endl;
				std::cout << "Continue loading OBJ? (y/n)";
				char response;

				while (true)
				{
					std::cin >> response;
					if (response == 'y')
					{
						continue;
					}
					else if (response == 'n')
					{
						return false;
					}
				}
			}

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if (line.substr(0, 3) == "vt ")
		{
			float u = 0.f;
			float v = 0.f;

			if (sscanf_s(line.c_str(), "vt %f %f", &u, &v) < 2)
			{
				std::cout << "ERROR: OBJ formatting error: " << line << std::endl;
				std::cout << "Continue loading OBJ? (y/n)";
				char response;

				while (true)
				{
					std::cin >> response;
					if (response == 'y')
					{
						continue;
					}
					else if (response == 'n')
					{
						return false;
					}
				}
			}

			uvs.push_back(u);
			uvs.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			float x = 0.f;
			float y = 0.f;
			float z = 0.f;

			if (sscanf_s(line.c_str(), "vn %f %f %f", &x, &y, &z) < 3)
			{
				std::cout << "ERROR: OBJ formatting error: " << line << std::endl;
				std::cout << "Continue loading OBJ? (y/n)";
				char response;

				while (true)
				{
					std::cin >> response;
					if (response == 'y')
					{
						continue;
					}
					else if (response == 'n')
					{
						return false;
					}
				}
			}

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
		else if (line.substr(0, 2) == "f ")
		{
			unsigned int v1v = 0, v1u = 0, v1n = 0;
			unsigned int v2v = 0, v2u = 0, v2n = 0;
			unsigned int v3v = 0, v3u = 0, v3n = 0;

			if (sscanf_s(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &v1v, &v1u, &v1n, &v2v, &v2u, &v2n, &v3v, &v3u, &v3n) < 9)
			{
				std::cout << "ERROR: OBJ formatting error: " << line << std::endl;
				std::cout << "Continue loading OBJ? (y/n)";
				char response;

				while (true)
				{
					std::cin >> response;
					if (response == 'y')
					{
						continue;
					}
					else if (response == 'n')
					{
						return false;
					}
				}
			}

			v1v--;
			objData.push_back(vertices[v1v * 3]);
			objData.push_back(vertices[(v1v * 3) + 1]);
			objData.push_back(vertices[(v1v * 3) + 2]);
			v1u--;
			objData.push_back(uvs[v1u * 2]);
			objData.push_back(uvs[(v1u * 2) + 1]);
			v1n--;
			objData.push_back(normals[v1n * 3]);
			objData.push_back(normals[(v1n * 3) + 1]);
			objData.push_back(normals[(v1n * 3) + 2]);

			v2v--;
			objData.push_back(vertices[v2v * 3]);
			objData.push_back(vertices[(v2v * 3) + 1]);
			objData.push_back(vertices[(v2v * 3) + 2]);
			v2u--;
			objData.push_back(uvs[v2u * 2]);
			objData.push_back(uvs[(v2u * 2) + 1]);
			v2n--;
			objData.push_back(normals[v2n * 3]);
			objData.push_back(normals[(v2n * 3) + 1]);
			objData.push_back(normals[(v2n * 3) + 2]);

			v3v--;
			objData.push_back(vertices[v3v * 3]);
			objData.push_back(vertices[(v3v * 3) + 1]);
			objData.push_back(vertices[(v3v * 3) + 2]);
			v3u--;
			objData.push_back(uvs[v3u * 2]);
			objData.push_back(uvs[(v3u * 2) + 1]);
			v3n--;
			objData.push_back(normals[v3n * 3]);
			objData.push_back(normals[(v3n * 3) + 1]);
			objData.push_back(normals[(v3n * 3) + 2]);
		}
		else
		{
			// ignore the line
		}
	}

	std::string newFile(fileName);
	newFile = "./bmf/" + newFile;
	newFile.pop_back();
	newFile.pop_back();
	newFile.pop_back();
	newFile += "bmf";

	FILE *binaryFile = nullptr;
	fopen_s(&binaryFile, newFile.c_str(), "wb");

	unsigned int size = 0;
	size = objData.size();
	fwrite(&size, sizeof(unsigned int), 1, binaryFile);

	fwrite(&objData[0], sizeof(GLfloat), size, binaryFile);

	fclose(binaryFile);
	
	return true;
}

bool hasEnding(std::string const &fullString, std::string const &ending)
{
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

int main()
{
	std::vector<std::string> files;

	GetFilesInDirectory(files, "./obj");

	for (auto it = files.begin(); it != files.end(); it++)
	{
		if (hasEnding((*it), ".obj"))
		{
			std::cout << "Converting " << (*it) << " to BMF format..." << std::endl;

				if (convertFile((*it)))
				{
					std::cout << "Successfully converted file " << (*it) << std::endl << std::endl;
				}
				else
				{
					std::cout << "Error converting file " << (*it) << std::endl << std::endl;
				}
		}
	}

	system("pause");
}