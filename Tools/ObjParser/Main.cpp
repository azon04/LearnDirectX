#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Typedefs
typedef struct  
{
	float x, y, z;
} VertexType;

typedef struct  
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
} FaceType;

// Functions prototype
bool ConvertDataStructure(char*, char*);

// Main
int main(int argc, char* argv[])
{
	bool result;
	char* filename;
	char* outputFilename;

	// Read in the name of the model file
	if (argc < 3)
	{
		std::cerr << "Please specify the file input" << std::endl;
		std::cerr << "Please specify the file output" << std::endl;
		return 0;
	}
	else
	{
		filename = argv[1];
		outputFilename = argv[2];
	}

	// Read the data from file into data structures and then ouput it in our model format
	result = ConvertDataStructure(filename, outputFilename);
	if (!result)
	{
		return -1;
	}

	// Notify the user the model has been converted
	cout << "File has been converted. " << std::endl;

	return 0;
}


bool ConvertDataStructure(char* filename, char* outputFileName)
{
	vector<VertexType> vertices;
	vector<VertexType> texcoords;
	vector<VertexType> normals;
	vector<FaceType> faces;

	ifstream fin;
	int vertexIndex, texCoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the indexes
	vertexIndex = 0;
	texCoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file
	fin.open(filename);

	// Check if it was successful in opening the file
	if (fin.fail() == true)
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	// Read in the vertices, texture coordinates and normals into the data structures
	// Important: Also convert to ledt hand coordinate system since Maya use right hand coordinate system
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices
			if (input == ' ')
			{
				VertexType vertex;
				fin >> vertex.x >> vertex.y >> vertex.z;

				// Invert the Z vertex to change to left hand system
				vertex.z *= -1.0f;
				vertices.push_back(vertex);
			}
			// Read in the texture uv coordinates
			else if (input == 't')
			{
				VertexType texCoord;
				fin >> texCoord.x >> texCoord.y;

				// Invert the V texture coordinates to left hand system
				texCoord.y = 1.0f - texCoord.y;
				texcoords.push_back(texCoord);
			}
			// Read in the normals
			else if (input == 'n')
			{
				VertexType normal;
				fin >> normal.x >> normal.y >> normal.z;

				// Invert the Z noemal to change to left hand system
				normal.z *= -1.0f;
				normals.push_back(normal);
			}
		}
		// Read in the faces
		else if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system
				FaceType face;
				fin >> face.vIndex3 >> input2 >> face.tIndex3 >> input2 >> face.nIndex3;
				fin >> face.vIndex2 >> input2 >> face.tIndex2 >> input2 >> face.nIndex2;
				fin >> face.vIndex1 >> input2 >> face.tIndex1 >> input2 >> face.nIndex1;
				faces.push_back(face);
			}
		}

		// Read in the remainder of the line
		while (input != '\n' && !fin.eof())
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line
		fin.get(input);
	}

	// Close the file
	fin.close();

	// Open the output file
	fout.open(outputFileName);
	if (fout.fail())
	{
		std::cerr << "Failed to open file to write:" << outputFileName << std::endl;
		return false;
	}

	// Write out the file header that our model format uses
	fout << "Vertex Count: " << faces.size() * 3 << endl;
	fout << endl;
	fout << "Data: " << endl;
	fout << endl;

	// Now loop thru all the faces and output the three vertices for each face
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << " " << vertices[vIndex].y << " " << vertices[vIndex].z << " ";
		fout << texcoords[tIndex].x << " " << texcoords[tIndex].y << " ";
		fout << normals[nIndex].x << " " << normals[nIndex].y << " " << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << " " << vertices[vIndex].y << " " << vertices[vIndex].z << " ";
		fout << texcoords[tIndex].x << " " << texcoords[tIndex].y << " ";
		fout << normals[nIndex].x << " " << normals[nIndex].y << " " << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << " " << vertices[vIndex].y << " " << vertices[vIndex].z << " ";
		fout << texcoords[tIndex].x << " " << texcoords[tIndex].y << " ";
		fout << normals[nIndex].x << " " << normals[nIndex].y << " " << normals[nIndex].z << endl;
	}

	// Close the output file
	fout.close();

	return true;
}