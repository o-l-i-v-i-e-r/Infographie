#include "tgaimage.h"
#include "Model.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>


const int width = 2000;
const int height = 2000;
const int depth = 2000;
float tab[height][width];
Vec3f camera(0,0,3);
Vec3f eye(1,1,5);
Vec3f center(0,0,0);
TGAImage texture(1024, 1024, 3);

void line(TGAImage &image, int x0, int y0, int z0, int x1, int y1,int z1, float s1, float s2, Vec2f tex1, Vec2f tex2) {
	bool steep = false;
	if (std::abs(x0-x1)<std::abs(y0-y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(s1,s2);
		std::swap(tex1, tex2);
	}
	int dx = x1-x0;
	int dy = y1-y0;
	int derrordx = 2*std::abs(dy);
	int errordx = 0;
	int y = y0;
	for (int x=x0; x<=x1; x++) {
	  float t;
	  if((x-x0)==(x1-x0)){t=1;}
	  else {
	    t = (x-x0)/(float)(x1-x0);}
	   int z = z0*(1.-t) + z1*t;
	   float s = s2*t + s1*(1.-t);
	   
	   TGAColor couleurTex = texture.get(tex2.x*t + tex1.x*(1.-t), tex2.y*t + tex1.y*(1.-t));
		
		if (steep) {
		  if(tab[y][x]<z) {
		    image.set(y, x, TGAColor(couleurTex.r*s, couleurTex.g*s, couleurTex.b*s ,3));
		    tab[y][x] = z;
		  }
		} else {
		  if(tab[x][y]<z) {
		   image.set(x, y, TGAColor(couleurTex.r*s, couleurTex.g*s, couleurTex.b*s, 3));
		    tab[x][y] = z;
		  }
		}
		errordx += derrordx;

		if (errordx>dx) {
			y += (y1>y0?1:-1);
			errordx -= 2*dx;
		}
	}
}



fmatrix viewport(int x, int y, int w, int h) {
  fmatrix m = fmatrix::identity(4);
  m[0][3] = x+w/2.f;
  m[1][3] = y+h/2.f;
  m[2][3] = depth/2.f;
  
  m[0][0] = w/2.f;
  m[1][1] = h/2.f;
  m[2][2] = depth/2.f;
  return m;
}

Vec3f matrixToVector(fmatrix m) {
	return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

fmatrix vectorToMatrix(Vec3f v) {
	fmatrix m(4, 1);
	m[0][0] = v.x;
	m[1][0] = v.y;
	m[2][0] = v.z;
	m[3][0] = 1.f;
return m;
}

fmatrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
  Vec3f z = (eye-center).normalize();
  Vec3f x = (up^z).normalize();
  Vec3f y = (z^x).normalize();
  fmatrix res = fmatrix::identity(4);
  for (int i=0; i<3; i++) {
    res[0][i] = x.raw[i];
    res[1][i] = y.raw[i];
    res[2][i] = z.raw[i];
    res[i][3] = -center.raw[i];
  }
  return res;
}


void triangle(TGAImage &image, Vec3f pt0, Vec3f pt1, Vec3f pt2, Vec3f nv0, Vec3f nv1, Vec3f nv2, Vec2f tex0, Vec2f tex1, Vec2f tex2) {
	int x0, x1, x2, y0, y1, y2, z0, z1, z2;  
	x0 = pt0.x; x1 = pt1.x; x2 = pt2.x;
	y0 = pt0.y; y1 = pt1.y; y2 = pt2.y;
	z0 = pt0.z; z1 = pt1.z; z2 = pt2.z;

	Vec3f nvLumiere(1,-1,1);
	
	float tailleLum = std::sqrt((nvLumiere.x*nvLumiere.x)+(nvLumiere.y*nvLumiere.y)+(nvLumiere.z*nvLumiere.z));
	nvLumiere.x = nvLumiere.x/tailleLum;
	nvLumiere.y = nvLumiere.y/tailleLum;
	nvLumiere.z = nvLumiere.z/tailleLum;
	float minLum = 0.1;
	float resultP0 = nvLumiere.x * nv0.x + nvLumiere.y * nv0.y + nvLumiere.z * nv0.z;
	if(resultP0 < minLum) {
		resultP0 = minLum;
	}
	float resultP1 = nvLumiere.x * nv1.x + nvLumiere.y * nv1.y + nvLumiere.z * nv1.z;
	if(resultP1 < minLum) {
		resultP1 = minLum;
	}
	float resultP2 = nvLumiere.x * nv2.x + nvLumiere.y * nv2.y + nvLumiere.z * nv2.z;
	if(resultP2 < minLum) {
		resultP2 = minLum;
	}
	
	if (x0>x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		std::swap(z0, z1);
		std::swap(resultP0, resultP1);
		std::swap(tex0, tex1);
	}
	if (x0>x2) {
		std::swap(x0, x2);
		std::swap(y0, y2);
		std::swap(z0, z2);
		std::swap(resultP0, resultP2);
		std::swap(tex0, tex2);
	}
	
	if(x2<x1) {
		std::swap(x1,x2);
		std::swap(y1,y2);
		std::swap(z1, z2);
		std::swap(resultP1, resultP2);
		std::swap(tex1, tex2);
	}

	for(int x = x0; x<x1; x++) {

	  float t1,t2;
	  if((x-x0)==(x1-x0)){t1=1;}
	  else {t1 = (x-x0)/(float)(x1-x0);}
	  if((x-x0)==(x2-x0)){t2=1;}
	  else {t2 = (x-x0)/(float)(x2-x0);}
	    float c1 = resultP1*t1 + resultP0*(1.-t1);
	    float c2 = resultP2*t2 + resultP0*(1.-t2);
	    int yP1 = (y1-y0)*t1 + y0;
	    int zP1 = (z1-z0)*t1 + z0;
	    int yP2 = (y2-y0)*t2 + y0;
	    int zP2 = (z2-z0)*t2 + z0;
	    Vec2f texture1(tex1.x*t1 + tex0.x*(1.-t1), tex1.y*t1 + tex0.y*(1.-t1));
	    Vec2f texture2(tex2.x*t2 + tex0.x*(1.-t2), tex2.y*t2 + tex0.y*(1.-t2));
	    
		line(image, x, yP1, zP1, x, yP2, zP2, c1, c2, texture1, texture2);
	}

	
	for(int x = x1; x<x2; x++) {

	  float t1,t2;
	  if((x-x1)==(x2-x1)){t1=1;}
	  else {t1 = (x-x1)/(float)(x2-x1);}
	  if((x-x0)==(x2-x0)){t2=1;}
	  else {t2 = (x-x0)/(float)(x2-x0);}
	  
	  	float c1 = resultP2*t1 + resultP1*(1.-t1);
		float c2 = resultP2*t2 + resultP0*(1.-t2);
		int yP1 = (y2-y1)*t1 + y1;
		int zP1 = (z2-z1)*t1 + z1;
		int yP2 = (y2-y0)*t2 + y0;
		int zP2 = (z2-z0)*t2 + z0;
	    Vec2f texture1(tex2.x*t1 + tex1.x*(1.-t1), tex2.y*t1 + tex1.y*(1.-t1));
	    Vec2f texture2(tex2.x*t2 + tex0.x*(1.-t2), tex2.y*t2 + tex0.y*(1.-t2));
		line(image, x, yP1, zP1, x, yP2, zP2, c1, c2, texture1, texture2);
	}
}

std::vector<Vec3f> toScale(Model m) {
	std::vector<Vec3f> pt;
	pt = m.getVertices();
	for(int i=0; i< m.nbVertices();i++) {
		pt[i].x = ((pt[i].x+1)/2)*width;
		pt[i].y = ((pt[i].y+1)/2)*height;
		pt[i].z = ((pt[i].z+1)/2)*depth;
	}
	return pt;
}

int main() {
  
  texture.read_tga_file("./obj/african_head_diffuse.tga");
  texture.flip_vertically();

  TGAImage image(width,height,3);
  Model m ("./obj/african_head.obj");
  for(int i=0; i<2000; i++){
	  for(int j=0; j<2000; j++) {
		tab[i][j] = -1 * INFINITY;
	  }
  }
  
  fmatrix projection = fmatrix::identity(4);
  fmatrix viewPort = viewport(width/8, height/8, width*3/4, height*3/4);
  fmatrix modelView = lookat(eye, center, Vec3f(0,1,0));
  projection[3][2] = -1.f/(eye-center).norm();
  
  std::vector<Vec3f> pt = m.getVertices();
  std::vector<Vec3i> fc = m.getFaces();
  std::vector<Vec3i> iNv = m.getIndexNv();
  std::vector<Vec3f> listNv = m.getNVectorList();
  std::vector<Vec3i> iVt = m.getIndexVt();
  std::vector<Vec3f> listVt = m.getVtList();
  
  
  	int idxPt0, idxPt1, idxPt2;
for(int i=0; i<m.nbFaces(); i++) {
	idxPt0 = fc[i].x;
	idxPt1 = fc[i].y;
	idxPt2 = fc[i].z;
	Vec3f pt0(pt[idxPt0].x, pt[idxPt0].y, pt[idxPt0].z);
	Vec3f pt1(pt[idxPt1].x, pt[idxPt1].y, pt[idxPt1].z);
	Vec3f pt2(pt[idxPt2].x, pt[idxPt2].y, pt[idxPt2].z);
	
	Vec3f projPt0 = matrixToVector(viewPort*projection*modelView*vectorToMatrix(pt0));
	Vec3f projPt1 = matrixToVector(viewPort*projection*modelView*vectorToMatrix(pt1));
	Vec3f projPt2 = matrixToVector(viewPort*projection*modelView*vectorToMatrix(pt2));
	
	Vec2f textPt0(listVt[iVt[i].x].x*1024, listVt[iVt[i].x].y*1024);
	Vec2f textPt1(listVt[iVt[i].y].x*1024, listVt[iVt[i].y].y*1024);
	Vec2f textPt2(listVt[iVt[i].z].x*1024, listVt[iVt[i].z].y*1024);
	
	triangle(image, projPt0, projPt1, projPt2, listNv[iNv[i].x],listNv[iNv[i].y],listNv[iNv[i].z], textPt0, textPt1, textPt2);
	
  }
	image.flip_vertically();
	image.write_tga_file("dump.tga");
	return 0;
}
