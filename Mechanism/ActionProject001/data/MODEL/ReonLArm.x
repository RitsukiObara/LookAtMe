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
 81;
 -0.00048;-0.01676;0.00000;,
 -0.00048;3.59492;-3.61166;,
 -0.00048;-0.01676;-5.10767;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;5.09090;0.00000;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;3.59492;3.61166;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;-0.01676;5.10767;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;-3.62841;3.61166;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;-5.12442;0.00000;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;-3.62841;-3.61166;,
 -0.00048;-0.01676;0.00000;,
 -0.00048;-0.01676;-5.10767;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-0.01662;-4.53144;,
 27.73863;3.18759;-3.20421;,
 27.73863;-0.01662;-0.00000;,
 27.73863;4.51481;0.00000;,
 27.73863;-0.01662;-0.00000;,
 27.73863;3.18759;3.20421;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-0.01664;4.53144;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-3.22085;3.20421;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-4.54808;-0.00000;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-3.22085;-3.20421;,
 27.73863;-0.01662;-0.00000;,
 27.73863;-0.01662;-4.53144;,
 23.47978;-0.01664;-5.10767;,
 23.47978;3.59504;-3.61166;,
 23.47978;-0.01664;-5.10767;,
 23.47978;-3.62829;-3.61166;,
 23.47978;5.09102;0.00000;,
 23.47978;-5.12430;-0.00000;,
 23.47978;3.59504;3.61166;,
 23.47978;-3.62829;3.61166;,
 23.47978;-0.01664;5.10767;,
 12.62340;-7.04606;-7.04627;,
 12.62342;7.04645;-7.04627;,
 12.62342;5.63721;-5.63702;,
 12.62340;-5.63684;-5.63702;,
 12.62342;7.04645;7.04627;,
 12.62342;5.63721;5.63702;,
 12.62340;-7.04606;7.04627;,
 12.62340;-5.63682;5.63702;,
 -0.00031;7.04709;-7.04700;,
 -0.00031;-7.04541;-7.04700;,
 -1.43319;-5.32896;-5.32917;,
 -1.43319;5.32935;-5.32917;,
 -1.43319;5.32935;5.32917;,
 -0.00031;7.04709;7.04553;,
 12.62342;7.04645;7.04627;,
 12.62342;7.04645;-7.04627;,
 -0.00031;-7.04541;-7.04700;,
 12.62340;-7.04606;-7.04627;,
 12.62340;-7.04606;7.04627;,
 -0.00031;-7.04541;7.04553;,
 -1.43319;-5.32896;5.32917;,
 -1.43319;-5.32896;-5.32917;,
 -0.00031;-5.63691;5.63700;,
 -0.00031;5.63712;5.63700;,
 12.62342;5.63721;5.63702;,
 12.62340;-5.63682;5.63702;,
 12.62340;-5.63684;-5.63702;,
 -0.00031;-5.63692;-5.63702;,
 -0.00031;5.63712;-5.63702;,
 12.62342;5.63721;-5.63702;,
 -0.00048;-0.01676;-5.10767;,
 -0.00048;-5.12442;0.00000;,
 -0.00048;-0.01676;5.10767;,
 -0.00048;5.09090;0.00000;,
 -0.00048;-3.62841;-3.61166;,
 -0.00048;3.59492;-3.61166;,
 -0.00048;3.59492;3.61166;,
 -0.00048;-3.62841;3.61166;;
 
 61;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,19;,
 3;20,19,21;,
 3;22,21,23;,
 3;24,23,25;,
 3;26,25,27;,
 3;28,27,29;,
 3;30,29,31;,
 3;32,31,33;,
 4;34,2,1,35;,
 4;34,35,19,18;,
 4;36,33,31,37;,
 4;36,37,14,16;,
 4;35,1,4,38;,
 4;35,38,21,19;,
 4;37,31,29,39;,
 4;37,39,12,14;,
 4;38,4,6,40;,
 4;38,40,23,21;,
 4;39,29,27,41;,
 4;39,41,10,12;,
 4;40,6,8,42;,
 4;40,42,25,23;,
 4;41,27,25,42;,
 4;41,42,8,10;,
 4;43,44,45,46;,
 4;44,47,48,45;,
 4;47,49,50,48;,
 4;49,43,46,50;,
 4;51,52,53,54;,
 4;51,54,55,56;,
 4;51,56,57,58;,
 4;59,60,61,62;,
 4;59,62,63,64;,
 4;56,55,63,62;,
 4;56,62,61,57;,
 4;65,66,67,68;,
 4;65,68,69,70;,
 4;66,71,72,67;,
 4;63,55,54,53;,
 3;73,70,71;,
 3;65,70,74;,
 3;65,75,66;,
 3;71,66,76;,
 3;70,73,77;,
 3;70,77,74;,
 3;71,78,73;,
 3;78,71,76;,
 3;66,79,76;,
 3;79,66,75;,
 3;80,75,65;,
 3;74,80,65;,
 4;70,69,45,71;,
 4;52,51,44,43;;
 
 MeshMaterialList {
  3;
  61;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  0,
  2,
  2,
  0,
  0,
  2,
  2,
  0,
  0,
  2,
  2,
  0,
  0,
  2,
  2,
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
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ReonSkin.png";
   }
  }
  Material {
   0.103200;0.103200;0.103200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.759200;0.708800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  57;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.067235;-0.000000;-0.997737;,
  0.067233;0.705509;-0.705505;,
  0.067237;-0.705507;-0.705507;,
  0.067232;0.997737;0.000000;,
  0.067238;-0.997737;-0.000000;,
  0.067233;0.705509;0.705505;,
  0.067237;-0.705507;0.705507;,
  0.067235;-0.000001;0.997737;,
  -0.941682;-0.238000;-0.237887;,
  -0.941716;0.237881;-0.237873;,
  -0.941678;0.237896;0.238009;,
  -0.941644;-0.238016;0.238023;,
  1.000000;-0.000001;0.000000;,
  1.000000;-0.000001;-0.000000;,
  -0.861334;0.359243;-0.359232;,
  -0.861251;-0.359423;-0.359252;,
  -0.423911;0.905704;0.000000;,
  -0.423790;-0.905761;0.000000;,
  0.000001;-0.000000;-1.000000;,
  -0.000007;1.000000;-0.000001;,
  0.000007;-1.000000;-0.000000;,
  1.000000;0.000000;0.000142;,
  1.000000;0.000146;-0.000000;,
  1.000000;0.000000;-0.000142;,
  1.000000;-0.000139;-0.000000;,
  1.000000;0.000041;0.000041;,
  1.000000;-0.000041;0.000041;,
  1.000000;-0.000041;-0.000041;,
  1.000000;0.000041;-0.000041;,
  0.000000;-0.000001;-1.000000;,
  -0.000004;0.707109;-0.707105;,
  0.134077;0.700724;-0.700721;,
  0.134078;0.000000;-0.990971;,
  0.134078;-0.700722;-0.700722;,
  0.000004;-0.707107;-0.707107;,
  -0.000005;1.000000;0.000000;,
  0.134076;0.990971;0.000000;,
  0.134078;-0.990971;-0.000000;,
  0.000005;-1.000000;-0.000000;,
  -0.000004;0.707109;0.707105;,
  0.134077;0.700724;0.700721;,
  0.134078;-0.700722;0.700722;,
  0.000004;-0.707107;0.707107;,
  0.000000;-0.000001;1.000000;,
  0.134078;-0.000001;0.990971;,
  0.000051;1.000000;0.000000;,
  -0.000051;-1.000000;0.000000;,
  -0.423781;0.000000;0.905765;,
  -0.000058;0.000000;1.000000;,
  1.000000;0.000101;0.000098;,
  1.000000;-0.000096;0.000098;,
  1.000000;0.000101;-0.000098;,
  1.000000;-0.000096;-0.000098;,
  0.000000;0.000000;1.000000;,
  0.000058;-0.000000;-1.000000;;
  61;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,31,32,3;,
  4;2,3,33,34;,
  4;2,34,35,4;,
  4;2,4,36,31;,
  4;3,32,37,5;,
  4;3,5,38,33;,
  4;4,35,39,6;,
  4;4,6,40,36;,
  4;5,37,41,7;,
  4;5,7,42,38;,
  4;6,39,43,8;,
  4;6,8,44,40;,
  4;7,41,45,9;,
  4;7,9,46,42;,
  4;8,43,46,9;,
  4;8,9,45,44;,
  4;14,14,14,14;,
  4;14,15,15,14;,
  4;15,15,15,15;,
  4;15,14,14,15;,
  4;16,17,10,11;,
  4;16,11,12,18;,
  4;47,18,47,47;,
  4;48,48,48,19;,
  4;17,19,13,10;,
  4;49,12,13,49;,
  4;49,49,50,50;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;13,12,11,10;,
  3;23,51,52;,
  3;53,51,24;,
  3;53,25,54;,
  3;52,54,26;,
  3;51,23,27;,
  3;51,27,24;,
  3;52,28,23;,
  3;28,52,26;,
  3;54,29,26;,
  3;29,54,25;,
  3;30,25,53;,
  3;24,30,53;,
  4;55,55,55,55;,
  4;56,56,56,56;;
 }
 MeshTextureCoords {
  81;
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.062500;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.187500;1.000000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.375000;1.000000;,
  0.437500;1.000000;,
  0.500000;1.000000;,
  0.562500;1.000000;,
  0.625000;1.000000;,
  0.687500;1.000000;,
  0.750000;1.000000;,
  0.812500;1.000000;,
  0.875000;1.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.000000;0.965230;,
  0.125000;0.965230;,
  1.000000;0.965230;,
  0.875000;0.965230;,
  0.250000;0.965230;,
  0.750000;0.965230;,
  0.375000;0.965230;,
  0.625000;0.965230;,
  0.500000;0.965230;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.250000;0.230180;,
  0.000000;0.230180;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.230180;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  1.000000;0.230180;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.230180;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.750000;0.321060;,
  0.500000;0.321060;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.321060;,
  0.250000;0.321060;,
  0.250000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}