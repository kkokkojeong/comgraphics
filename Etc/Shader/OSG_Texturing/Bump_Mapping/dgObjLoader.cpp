#include "dgObjLoader.h"

/* -------------------------------------

	Constructor & Destructor

   -------------------------------------*/
ObjLoader::ObjLoader()
{
	m_Vertices.clear();
	m_Normals.clear();
	m_Faces.clear();
	m_NormalIDX.clear();
	m_TexCoord.clear();
}
ObjLoader::~ObjLoader()
{
	m_Vertices.clear();
	m_Normals.clear();
	m_Faces.clear();
	m_NormalIDX.clear();
	m_TexCoord.clear();
}

/* -------------------------------------

	Member Functions

   -------------------------------------*/

bool ObjLoader::LoadObj(char* _fileName)
{
	
	ifstream readStream(_fileName);
	if(!readStream.is_open()) return false;
	cout << " Loading : .... " << _fileName << endl;
	string tempStr;
	string checkScene;
	string checkFace;

	gPoint3D tempData;
	gPoint3D texIdx;
	gPoint3D faceIdx;
	gPoint3D normIdx;

	int count = 0;

	// Load Obj file 
	while(getline(readStream, tempStr)) {
		istringstream checkParse(tempStr);
		checkParse >> checkScene;	

		// Load Vertices
		if(checkScene == "v") {	
			checkParse >> tempData.x >> tempData.y >> tempData.z;
			m_Vertices.push_back(tempData);
		}

		// Load Vertices normal
		if(checkScene == "vn") {
			checkParse >> tempData.x >> tempData.y >> tempData.z;
			m_Normals.push_back(tempData);
		}

		// Load Vertices texture coordinate
		if(checkScene == "vt") {
			tempData.x = 0.0; tempData.y = 0.0;  tempData.z = 0.0; 
			checkParse >> tempData.x >> tempData.y;
			//tempModel.setTexVert(tempData);
		}

		//  v/vt/vn or v//vn or v/vt  : Load Face Idx
		if(checkScene == "f") { 
			for(int i = 0; i < 3; i++) {
				checkParse >> checkFace; // Parsed data

				// Check white space
				while(checkFace.find("/") != string::npos) {
					int parseIDX = checkFace.find("/");
					checkFace.erase(parseIDX, 1);
					checkFace.insert(parseIDX, " ");
					count += 1;
				}
				istringstream faceParse(checkFace);

				for(int j = 0; j < (count+1); j ++) {
					if(j == 0) {
						faceParse >> tempData.x;
					} else if(j == 1) {
						faceParse >> tempData.y;
					} else if(j == 2) {
						faceParse >> tempData.z;
					}					
				}


				if(i == 0) {
					faceIdx.x = tempData.x -1;
					texIdx.x  = tempData.y -1;
					normIdx.x = tempData.z -1;		
				} else if(i == 1) {
					faceIdx.y = tempData.x -1;
					texIdx.y  = tempData.y -1;
					normIdx.y = tempData.z -1;	
				} else if(i == 2) {
					faceIdx.z = tempData.x -1;
					texIdx.z  = tempData.y -1;
					normIdx.z = tempData.z -1;	
				}


				count = 0;
			}
			m_Faces.push_back(faceIdx);		
			m_NormalIDX.push_back(normIdx);
		}

	}
	readStream.close();


	cout << " Size of Vertices : " << m_Vertices.size() << endl;
	cout << " Size of Normals : "  << m_Normals.size() << endl;
	cout << " Size of faces : "    << m_Faces.size() << endl;
	
	
	
	return true;
}

void ObjLoader::CalculateNormal()
{

	// Todo calculate Normal vector
}


void ObjLoader::normalize(float v[3])
{
	float r;

	r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	if (r == 0.0) return;

	v[0] /= r;
	v[1] /= r;
	v[2] /= r;
}

void ObjLoader::cross(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1]*v2[2] - v1[2]*v2[1];
	result[1] = v1[2]*v2[0] - v1[0]*v2[2];
	result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

int ObjLoader::GetNumberOfVertices() const
{
	return m_Vertices.size();
}

int ObjLoader::GetNumberOfFaces() const
{
	return m_Faces.size();
}

double* ObjLoader::GetVertices(const int _idx) const
{
	assert(_idx >= 0 && _idx < m_Vertices.size());

	double vertices[3];
	gPoint3D temp;

	temp = m_Vertices[_idx];
	vertices[0] = temp.x; vertices[1] = temp.y; vertices[2] = temp.z;

	return vertices;
}
double* ObjLoader::GetNormals(const int _idx) const
{
	assert(_idx >= 0 && _idx < m_Normals.size());

	double normal[3];
	gPoint3D temp;

	temp = m_Normals[_idx];
	normal[0] = temp.x; normal[1] = temp.y; normal[2] = temp.z;

	return normal;

}
int* ObjLoader::GetFace(const int _idx) const
{
	assert(_idx >= 0 && _idx < m_Faces.size());

	int face[3];
	gPoint3D temp;
	temp = m_Faces[_idx];
	face[0] = (int)temp.x; face[1] = (int)temp.y; face[2] = (int)temp.z;

	return face;

}
int * ObjLoader::GetNormIDX(const int _idx) const
{
	assert(_idx >= 0 && _idx < m_Faces.size());

	int face[3];
	gPoint3D temp;
	temp = m_NormalIDX[_idx];
	face[0] = (int)temp.x; face[1] = (int)temp.y; face[2] = (int)temp.z;

	return face;
}

GLint ObjLoader::GetDisplayList()
{
	
 	int *face, *normIDX;


	GLint objID=glGenLists(1);
	glNewList(objID, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < GetNumberOfFaces(); i++) {			
		int idx, idy, idz;
		face = GetFace(i);
		idx = face[0];
		idy = face[1];
		idz = face[2];

		normIDX = GetNormIDX(i);
		int nidx, nidy, nidz;
		nidx = normIDX[0];
		nidy = normIDX[1];
		nidz = normIDX[2];

		//glNormal3dv( objLoader->GetNormals(i) );
		glNormal3dv( GetNormals(nidx) );
		glVertex3dv( GetVertices(idx) );

		glNormal3dv( GetNormals(nidy) );
		glVertex3dv( GetVertices(idy) );

		glNormal3dv( GetNormals(nidz) );
		glVertex3dv( GetVertices(idz) );
	}
	glEnd();		

	glEndList();


	return objID;
}