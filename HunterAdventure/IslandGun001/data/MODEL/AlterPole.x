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
 54;
 -0.00870;599.50081;-99.64470;,
 70.44540;599.50081;-70.46160;,
 70.44540;34.04910;-70.46160;,
 -0.00870;34.04910;-99.64470;,
 99.62850;599.50081;-0.00750;,
 99.62850;34.04910;-0.00750;,
 70.44540;599.50081;70.44660;,
 70.44540;34.04910;70.44660;,
 -0.00870;599.50081;99.62970;,
 -0.00870;34.04910;99.62970;,
 -70.46280;599.50081;70.44660;,
 -70.46280;34.04910;70.44660;,
 -99.64560;599.50081;-0.00750;,
 -99.64560;34.04910;-0.00750;,
 -70.46280;599.50081;-70.46160;,
 -70.46280;34.04910;-70.46160;,
 -0.00870;599.50081;-99.64470;,
 -0.00870;34.04910;-99.64470;,
 88.17330;0.00870;-88.18950;,
 -0.00870;0.00870;-124.71540;,
 124.69950;0.00870;-0.00750;,
 88.17330;0.00870;88.17450;,
 -0.00870;0.00870;124.70070;,
 -88.19040;0.00870;88.17450;,
 -124.71660;0.00870;-0.00750;,
 -88.19040;0.00870;-88.18950;,
 -0.00870;0.00870;-124.71540;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;0.00870;-0.00750;,
 -0.00870;599.50081;-134.64600;,
 95.19510;599.50081;-95.21130;,
 -0.00870;646.84442;-134.64600;,
 95.19510;646.84442;-95.21130;,
 134.62980;599.50081;-0.00750;,
 134.62980;646.84442;-0.00750;,
 95.19510;599.50081;95.19630;,
 95.19510;646.84442;95.19630;,
 -0.00870;599.50081;134.63100;,
 -0.00870;646.84442;134.63100;,
 -95.21250;599.50081;95.19630;,
 -95.21250;646.84442;95.19630;,
 -134.64690;599.50081;-0.00750;,
 -134.64690;646.84442;-0.00750;,
 -95.21250;599.50081;-95.21130;,
 -95.21250;646.84442;-95.21130;,
 -0.00870;599.50081;-134.64600;,
 -0.00870;646.84442;-134.64600;,
 -0.00870;646.84442;-0.00750;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 3;27,19,18;,
 3;28,18,20;,
 3;29,20,21;,
 3;30,21,22;,
 3;31,22,23;,
 3;32,23,24;,
 3;33,24,25;,
 3;34,25,26;,
 4;35,36,1,0;,
 4;37,38,36,35;,
 4;36,39,4,1;,
 4;38,40,39,36;,
 4;39,41,6,4;,
 4;40,42,41,39;,
 4;41,43,8,6;,
 4;42,44,43,41;,
 4;43,45,10,8;,
 4;44,46,45,43;,
 4;45,47,12,10;,
 4;46,48,47,45;,
 4;47,49,14,12;,
 4;48,50,49,47;,
 4;49,51,16,14;,
 4;50,52,51,49;,
 3;38,52,53;,
 3;53,52,50;,
 3;50,48,53;,
 3;48,46,53;,
 3;46,44,53;,
 3;44,42,53;,
 3;53,42,40;,
 3;53,40,38;;
 
 MeshMaterialList {
  3;
  48;
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
    "data\\TEXTURE\\SilverGloss.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\AlterPole.png";
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
  32;
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707108;0.000000;0.707105;,
  -1.000000;0.000000;0.000000;,
  -0.707108;0.000000;-0.707105;,
  -0.000000;0.316223;-0.948685;,
  0.670821;0.316224;-0.670822;,
  0.948684;0.316225;0.000000;,
  0.670821;0.316225;0.670821;,
  -0.000000;0.316224;0.948684;,
  -0.670822;0.316224;0.670820;,
  -0.948685;0.316223;0.000000;,
  -0.670822;0.316223;-0.670821;,
  -0.000000;0.593022;-0.805186;,
  0.569351;0.593024;-0.569352;,
  0.805184;0.593025;0.000000;,
  0.569351;0.593024;0.569351;,
  -0.000000;0.593024;0.805185;,
  -0.569353;0.593023;0.569351;,
  -0.805186;0.593022;0.000000;,
  -0.569353;0.593022;-0.569353;,
  0.000000;-1.000000;-0.000000;,
  -0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -0.000000;0.000000;1.000000;,
  -0.707108;0.000000;0.707106;,
  -1.000000;0.000000;0.000000;,
  -0.707108;0.000000;-0.707106;;
  48;
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  3;25,25,25;,
  4;25,25,25,25;,
  4;26,2,2,26;,
  4;25,25,25,25;,
  4;2,27,27,2;,
  4;25,25,25,25;,
  4;27,4,4,27;,
  4;25,25,25,25;,
  4;4,28,28,4;,
  4;25,25,25,25;,
  4;28,29,29,28;,
  4;25,25,25,25;,
  4;29,30,30,29;,
  4;25,25,25,25;,
  4;30,31,31,30;,
  4;25,25,25,25;,
  4;31,26,26,31;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;;
 }
 MeshTextureCoords {
  54;
  0.000000;0.333330;,
  0.125000;0.333330;,
  0.125000;0.666670;,
  0.000000;0.666670;,
  0.250000;0.333330;,
  0.250000;0.666670;,
  0.375000;0.333330;,
  0.375000;0.666670;,
  0.500000;0.333330;,
  0.500000;0.666670;,
  0.625000;0.333330;,
  0.625000;0.666670;,
  0.750000;0.333330;,
  0.750000;0.666670;,
  0.875000;0.333330;,
  0.875000;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.333330;,
  0.125000;0.333330;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.250000;0.333330;,
  0.250000;0.000000;,
  0.375000;0.333330;,
  0.375000;0.000000;,
  0.500000;0.333330;,
  0.500000;0.000000;,
  0.625000;0.333330;,
  0.625000;0.000000;,
  0.750000;0.333330;,
  0.750000;0.000000;,
  0.875000;0.333330;,
  0.875000;0.000000;,
  1.000000;0.333330;,
  1.000000;0.000000;,
  0.187500;0.000000;;
 }
}
