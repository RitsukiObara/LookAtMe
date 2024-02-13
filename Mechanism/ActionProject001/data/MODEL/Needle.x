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
 107;
 -0.00000;0.00000;-39.99515;,
 0.00000;80.00000;0.00000;,
 28.28083;0.00000;-28.28083;,
 0.00000;80.00000;0.00000;,
 39.99515;0.00000;0.00000;,
 0.00000;80.00000;0.00000;,
 28.28083;0.00000;28.28084;,
 0.00000;80.00000;0.00000;,
 0.00000;0.00000;39.99515;,
 0.00000;80.00000;0.00000;,
 -28.28083;0.00000;28.28084;,
 0.00000;80.00000;0.00000;,
 -39.99515;0.00000;0.00000;,
 0.00000;80.00000;0.00000;,
 -28.28083;0.00000;-28.28083;,
 0.00000;80.00000;0.00000;,
 -0.00000;0.00000;-39.99515;,
 0.00000;0.00000;0.00000;,
 -0.00000;0.00000;-39.99515;,
 28.28083;0.00000;-28.28083;,
 39.99515;0.00000;0.00000;,
 28.28083;0.00000;28.28084;,
 0.00000;0.00000;39.99515;,
 -28.28083;0.00000;28.28084;,
 -39.99515;0.00000;0.00000;,
 -28.28083;0.00000;-28.28083;,
 39.99915;0.00000;0.00000;,
 41.46355;3.53546;0.00000;,
 29.31917;3.53546;-29.31917;,
 28.28366;0.00000;-28.28366;,
 44.99903;4.99989;0.00000;,
 31.81912;4.99989;-31.81912;,
 48.53448;3.53546;0.00000;,
 34.31908;3.53546;-34.31908;,
 49.99893;-0.00000;-0.00000;,
 35.35458;-0.00000;-35.35458;,
 48.53448;-3.53546;-0.00000;,
 34.31908;-3.53546;-34.31908;,
 44.99903;-4.99989;-0.00000;,
 31.81912;-4.99989;-31.81912;,
 41.46355;-3.53546;-0.00000;,
 29.31917;-3.53546;-29.31917;,
 39.99915;0.00000;0.00000;,
 28.28366;0.00000;-28.28366;,
 -0.00000;3.53546;-41.46355;,
 -0.00000;0.00000;-39.99915;,
 -0.00000;4.99989;-44.99903;,
 -0.00000;3.53546;-48.53448;,
 -0.00000;-0.00000;-49.99893;,
 -0.00000;-3.53546;-48.53448;,
 -0.00000;-4.99989;-44.99903;,
 -0.00000;-3.53546;-41.46355;,
 -0.00000;0.00000;-39.99915;,
 -29.31917;3.53546;-29.31917;,
 -28.28366;0.00000;-28.28366;,
 -31.81912;4.99989;-31.81912;,
 -34.31908;3.53546;-34.31908;,
 -35.35458;-0.00000;-35.35458;,
 -34.31908;-3.53546;-34.31908;,
 -31.81912;-4.99989;-31.81912;,
 -29.31917;-3.53546;-29.31917;,
 -28.28366;0.00000;-28.28366;,
 -41.46355;3.53546;0.00000;,
 -39.99915;0.00000;0.00000;,
 -44.99903;4.99989;0.00000;,
 -48.53448;3.53546;0.00000;,
 -49.99893;-0.00000;0.00000;,
 -48.53448;-3.53546;0.00000;,
 -44.99903;-4.99989;0.00000;,
 -41.46355;-3.53546;0.00000;,
 -39.99915;0.00000;0.00000;,
 -29.31916;3.53546;29.31917;,
 -28.28366;0.00000;28.28367;,
 -31.81912;4.99989;31.81912;,
 -34.31907;3.53546;34.31908;,
 -35.35457;-0.00000;35.35458;,
 -34.31907;-3.53546;34.31908;,
 -31.81912;-4.99989;31.81912;,
 -29.31916;-3.53546;29.31917;,
 -28.28366;0.00000;28.28367;,
 0.00000;3.53546;41.46355;,
 0.00000;0.00000;39.99915;,
 0.00000;4.99989;44.99903;,
 0.00000;3.53546;48.53448;,
 0.00000;-0.00000;49.99893;,
 0.00000;-3.53546;48.53448;,
 0.00000;-4.99989;44.99903;,
 0.00000;-3.53546;41.46355;,
 0.00000;0.00000;39.99915;,
 29.31917;3.53546;29.31917;,
 28.28366;0.00000;28.28367;,
 31.81912;4.99989;31.81912;,
 34.31907;3.53546;34.31908;,
 35.35458;-0.00000;35.35458;,
 34.31907;-3.53546;34.31908;,
 31.81912;-4.99989;31.81912;,
 29.31917;-3.53546;29.31917;,
 28.28366;0.00000;28.28367;,
 41.46355;3.53546;0.00000;,
 39.99915;0.00000;0.00000;,
 44.99903;4.99989;0.00000;,
 48.53448;3.53546;0.00000;,
 49.99893;-0.00000;-0.00000;,
 48.53448;-3.53546;-0.00000;,
 44.99903;-4.99989;-0.00000;,
 41.46355;-3.53546;-0.00000;,
 39.99915;0.00000;0.00000;;
 
 80;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;,
 4;26,27,28,29;,
 4;27,30,31,28;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;29,28,44,45;,
 4;28,31,46,44;,
 4;31,33,47,46;,
 4;33,35,48,47;,
 4;35,37,49,48;,
 4;37,39,50,49;,
 4;39,41,51,50;,
 4;41,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 4;63,62,71,72;,
 4;62,64,73,71;,
 4;64,65,74,73;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;69,70,79,78;,
 4;72,71,80,81;,
 4;71,73,82,80;,
 4;73,74,83,82;,
 4;74,75,84,83;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 4;77,78,87,86;,
 4;78,79,88,87;,
 4;81,80,89,90;,
 4;80,82,91,89;,
 4;82,83,92,91;,
 4;83,84,93,92;,
 4;84,85,94,93;,
 4;85,86,95,94;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;90,89,98,99;,
 4;89,91,100,98;,
 4;91,92,101,100;,
 4;92,93,102,101;,
 4;93,94,103,102;,
 4;94,95,104,103;,
 4;95,96,105,104;,
 4;96,97,106,105;;
 
 MeshMaterialList {
  2;
  80;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.752800;0.094400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.545882;0.470588;0.363922;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  73;
  -0.000000;0.447170;-0.894449;,
  0.632471;0.447170;-0.632471;,
  0.894449;0.447170;-0.000000;,
  0.632471;0.447170;0.632471;,
  -0.000000;0.447170;0.894449;,
  -0.632471;0.447170;0.632471;,
  -0.894449;0.447170;0.000000;,
  -0.632471;0.447170;-0.632471;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.698873;0.715245;0.000000;,
  0.000001;1.000000;0.000000;,
  0.698872;0.715247;0.000000;,
  1.000000;-0.000000;-0.000000;,
  0.698872;-0.715247;-0.000000;,
  0.000001;-1.000000;0.000000;,
  -0.698873;-0.715245;0.000000;,
  -0.707107;0.000000;0.707107;,
  -0.494178;0.715245;0.494178;,
  0.000000;1.000000;-0.000000;,
  0.494177;0.715247;-0.494177;,
  0.707107;-0.000000;-0.707107;,
  0.494177;-0.715247;-0.494177;,
  0.000000;-1.000000;-0.000000;,
  -0.494178;-0.715245;0.494178;,
  0.000000;0.000000;1.000000;,
  0.000000;0.715245;0.698873;,
  0.000000;1.000000;-0.000001;,
  -0.000000;0.715247;-0.698872;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.715247;-0.698872;,
  0.000000;-1.000000;-0.000001;,
  0.000000;-0.715245;0.698873;,
  0.707107;0.000000;0.707107;,
  0.494178;0.715245;0.494178;,
  -0.000000;1.000000;-0.000000;,
  -0.494177;0.715247;-0.494177;,
  -0.707107;-0.000000;-0.707107;,
  -0.494177;-0.715247;-0.494177;,
  -0.000000;-1.000000;-0.000000;,
  0.494178;-0.715245;0.494178;,
  1.000000;0.000000;-0.000000;,
  0.698873;0.715245;-0.000000;,
  -0.000001;1.000000;0.000000;,
  -0.698872;0.715247;0.000000;,
  -1.000000;-0.000000;0.000000;,
  -0.698872;-0.715247;0.000000;,
  -0.000001;-1.000000;0.000000;,
  0.698873;-0.715245;-0.000000;,
  0.707107;0.000000;-0.707107;,
  0.494178;0.715245;-0.494178;,
  -0.000001;1.000000;0.000000;,
  -0.494177;0.715247;0.494177;,
  -0.707107;-0.000000;0.707107;,
  -0.494177;-0.715247;0.494177;,
  -0.000001;-1.000000;0.000000;,
  0.494178;-0.715245;-0.494178;,
  -0.000000;0.000000;-1.000000;,
  -0.000000;0.715245;-0.698874;,
  -0.000000;1.000000;0.000000;,
  -0.000000;0.715247;0.698872;,
  0.000000;-0.000000;1.000000;,
  0.000000;-0.715247;0.698872;,
  0.000000;-1.000000;0.000001;,
  0.000000;-0.715245;-0.698873;,
  -0.707107;0.000000;-0.707107;,
  -0.494178;0.715245;-0.494178;,
  0.000000;1.000000;0.000000;,
  0.494177;0.715247;0.494177;,
  0.707107;-0.000000;0.707107;,
  0.494177;-0.715247;0.494177;,
  0.000000;-1.000000;0.000000;,
  -0.494178;-0.715245;-0.494178;;
  80;
  3;0,1,1;,
  3;1,1,2;,
  3;2,3,3;,
  3;3,3,4;,
  3;4,5,5;,
  3;5,5,6;,
  3;6,7,7;,
  3;7,7,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,49,57,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,57,65,72;,
  4;65,66,10,9;,
  4;66,67,11,10;,
  4;67,68,12,11;,
  4;68,69,13,12;,
  4;69,70,14,13;,
  4;70,71,15,14;,
  4;71,72,16,15;,
  4;72,65,9,16;;
 }
 MeshTextureCoords {
  107;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.125000;,
  0.125000;0.125000;,
  0.125000;0.000000;,
  0.000000;0.250000;,
  0.125000;0.250000;,
  0.000000;0.375000;,
  0.125000;0.375000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.000000;0.625000;,
  0.125000;0.625000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;0.125000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.250000;0.375000;,
  0.250000;0.500000;,
  0.250000;0.625000;,
  0.250000;0.750000;,
  0.250000;0.875000;,
  0.250000;1.000000;,
  0.375000;0.125000;,
  0.375000;0.000000;,
  0.375000;0.250000;,
  0.375000;0.375000;,
  0.375000;0.500000;,
  0.375000;0.625000;,
  0.375000;0.750000;,
  0.375000;0.875000;,
  0.375000;1.000000;,
  0.500000;0.125000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.500000;0.375000;,
  0.500000;0.500000;,
  0.500000;0.625000;,
  0.500000;0.750000;,
  0.500000;0.875000;,
  0.500000;1.000000;,
  0.625000;0.125000;,
  0.625000;0.000000;,
  0.625000;0.250000;,
  0.625000;0.375000;,
  0.625000;0.500000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.625000;1.000000;,
  0.750000;0.125000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  0.750000;0.375000;,
  0.750000;0.500000;,
  0.750000;0.625000;,
  0.750000;0.750000;,
  0.750000;0.875000;,
  0.750000;1.000000;,
  0.875000;0.125000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.875000;0.375000;,
  0.875000;0.500000;,
  0.875000;0.625000;,
  0.875000;0.750000;,
  0.875000;0.875000;,
  0.875000;1.000000;,
  1.000000;0.125000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  1.000000;0.375000;,
  1.000000;0.500000;,
  1.000000;0.625000;,
  1.000000;0.750000;,
  1.000000;0.875000;,
  1.000000;1.000000;;
 }
}
