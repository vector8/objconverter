#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL\glew.h>

bool convertFile(const std::string &fileName)
{
	std::ifstream in(fileName, std::ios::in);
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
	newFile.pop_back();
	newFile.pop_back();
	newFile.pop_back();
	newFile += "bmf";

	FILE *binaryFile = nullptr;
	fopen_s(&binaryFile, newFile.c_str(), "wb");

	fwrite(&objData[0], sizeof(GLfloat), objData.size(), binaryFile);

	fclose(binaryFile);
}

int main()
{
	std::cout << "Path of OBJ file? ";

	std::string fileName;

	std::cin >> fileName;

	if (convertFile(fileName))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}