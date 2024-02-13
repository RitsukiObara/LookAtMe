xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 0.00000;-0.00403;13.81614;,
 9.28434;-0.00403;9.97035;,
 9.28434;73.08314;9.97035;,
 0.00000;73.08314;13.81614;,
 13.13000;-0.00403;0.68601;,
 13.13000;73.08314;0.68601;,
 9.28434;-0.00403;-8.59820;,
 9.28434;73.08314;-8.59820;,
 0.00000;-0.00403;-12.44386;,
 0.00000;73.08314;-12.44386;,
 -9.28434;-0.00403;-8.59820;,
 -9.28434;73.08314;-8.59820;,
 -13.13000;-0.00403;0.68601;,
 -13.13000;73.08314;0.68601;,
 -9.28434;-0.00403;9.97035;,
 -9.28434;73.08314;9.97035;,
 0.00000;-0.00403;13.81614;,
 0.00000;73.08314;13.81614;,
 0.00000;-0.00403;-0.00299;,
 0.00000;73.08314;0.68601;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;18,4,1;,
 3;18,6,4;,
 3;18,8,6;,
 3;18,10,8;,
 3;18,12,10;,
 3;18,14,12;,
 3;18,0,14;,
 3;19,3,2;,
 3;19,2,5;,
 3;19,5,7;,
 3;19,7,9;,
 3;19,9,11;,
 3;19,11,13;,
 3;19,13,15;,
 3;19,15,3;;
 
 MeshMaterialList {
  2;
  24;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Fuse.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.707111;0.000000;0.707103;,
  1.000000;0.000000;-0.000002;,
  0.707105;0.000000;-0.707109;,
  0.000000;0.000000;-1.000000;,
  -0.707105;0.000000;-0.707109;,
  -1.000000;0.000000;-0.000002;,
  -0.707111;0.000000;0.707103;,
  0.000000;1.000000;0.000000;;
  24;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;;
 }
 MeshTextureCoords {
  20;
  0.000000;-0.643870;,
  0.127280;-0.643870;,
  0.127280;1.572370;,
  0.000000;1.572370;,
  0.253180;-0.643870;,
  0.253180;1.572370;,
  0.377220;-0.643870;,
  0.377220;1.572370;,
  0.500000;-0.643870;,
  0.500000;1.572370;,
  0.622780;-0.643870;,
  0.622780;1.572370;,
  0.746820;-0.643870;,
  0.746820;1.572370;,
  0.872720;-0.643870;,
  0.872720;1.572370;,
  1.000000;-0.643870;,
  1.000000;1.572370;,
  0.500000;-0.643870;,
  0.500000;1.572370;;
 }
}
