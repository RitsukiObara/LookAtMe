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
 98;
 -0.00100;5.94535;0.00000;,
 2.83817;4.21500;0.00000;,
 2.83817;2.98070;-2.97985;,
 -0.00100;4.20423;-4.20337;,
 -0.00100;0.00088;-5.94448;,
 2.83817;2.98070;-2.97985;,
 2.83817;0.00088;-4.21413;,
 2.83817;0.00088;-4.21413;,
 2.83817;-2.97898;-2.97985;,
 -0.00100;-4.20253;-4.20337;,
 -0.00100;0.00088;-5.94448;,
 -0.00100;-5.94362;0.00000;,
 2.83817;-2.97898;-2.97985;,
 2.83817;-4.21327;-0.00000;,
 -0.00100;-4.20253;4.20337;,
 2.83817;-4.21327;-0.00000;,
 2.83817;-2.97898;2.97985;,
 -0.00100;0.00088;5.94448;,
 2.83817;-2.97898;2.97985;,
 2.83817;0.00088;4.21413;,
 -0.00100;4.20423;4.20337;,
 2.83817;0.00088;4.21413;,
 2.83817;2.98070;2.97985;,
 2.83817;2.98070;2.97985;,
 2.83817;4.21500;0.00000;,
 4.52767;0.00088;2.10707;,
 5.03590;0.00088;0.00000;,
 4.52767;1.49080;1.48993;,
 4.52767;-1.48907;1.48993;,
 5.03590;0.00088;0.00000;,
 4.52767;0.00088;2.10707;,
 4.52767;-2.10623;-0.00000;,
 5.03590;0.00088;0.00000;,
 4.52767;-1.48907;1.48993;,
 4.52767;-1.48907;-1.48993;,
 5.03590;0.00088;0.00000;,
 4.52767;-2.10623;-0.00000;,
 4.52767;0.00088;-2.10707;,
 5.03590;0.00088;0.00000;,
 4.52767;-1.48907;-1.48993;,
 4.52767;1.49080;-1.48993;,
 5.03590;0.00088;0.00000;,
 4.52767;0.00088;-2.10707;,
 4.52767;2.10792;0.00000;,
 4.52767;1.49080;1.48993;,
 5.03590;0.00088;0.00000;,
 4.52767;2.10792;0.00000;,
 5.03590;0.00088;0.00000;,
 4.52767;1.49080;-1.48993;,
 -30.12088;4.20423;-4.20337;,
 -32.96012;2.98070;-2.97985;,
 -32.96012;4.21500;0.00000;,
 -30.12088;5.94535;0.00000;,
 -32.96012;0.00088;-4.21413;,
 -32.96012;2.98070;-2.97985;,
 -30.12088;0.00088;-5.94448;,
 -30.12088;0.00088;-5.94448;,
 -30.12088;-4.20253;-4.20337;,
 -32.96012;-2.97898;-2.97985;,
 -32.96012;0.00088;-4.21413;,
 -32.96012;-4.21325;0.00000;,
 -32.96012;-2.97898;-2.97985;,
 -30.12088;-5.94362;0.00000;,
 -32.96012;-2.97898;2.97985;,
 -32.96012;-4.21325;0.00000;,
 -30.12088;-4.20253;4.20337;,
 -32.96012;0.00088;4.21413;,
 -32.96012;-2.97898;2.97985;,
 -30.12088;0.00088;5.94448;,
 -32.96012;2.98070;2.97985;,
 -32.96012;0.00088;4.21413;,
 -30.12088;4.20423;4.20337;,
 -32.96012;4.21500;0.00000;,
 -32.96012;2.98070;2.97985;,
 -34.64962;1.49080;1.48993;,
 -35.15785;0.00088;0.00000;,
 -34.64962;0.00088;2.10707;,
 -34.64962;0.00088;2.10707;,
 -35.15785;0.00088;0.00000;,
 -34.64962;-1.48907;1.48993;,
 -34.64962;-1.48907;1.48993;,
 -35.15785;0.00088;0.00000;,
 -34.64962;-2.10620;0.00000;,
 -34.64962;-2.10620;0.00000;,
 -35.15785;0.00088;0.00000;,
 -34.64962;-1.48907;-1.48993;,
 -34.64962;-1.48907;-1.48993;,
 -35.15785;0.00088;0.00000;,
 -34.64962;0.00088;-2.10707;,
 -34.64962;0.00088;-2.10707;,
 -35.15785;0.00088;0.00000;,
 -34.64962;1.49080;-1.48993;,
 -35.15785;0.00088;0.00000;,
 -34.64962;1.49080;1.48993;,
 -34.64962;2.10792;0.00000;,
 -34.64962;1.49080;-1.48993;,
 -35.15785;0.00088;0.00000;,
 -34.64962;2.10792;0.00000;;
 
 56;
 4;0,1,2,3;,
 4;4,3,5,6;,
 4;7,8,9,10;,
 4;11,9,12,13;,
 4;14,11,15,16;,
 4;17,14,18,19;,
 4;20,17,21,22;,
 4;0,20,23,24;,
 3;25,26,27;,
 3;28,29,30;,
 3;31,32,33;,
 3;34,35,36;,
 3;37,38,39;,
 3;40,41,42;,
 3;43,44,45;,
 3;46,47,48;,
 4;25,27,22,21;,
 4;19,18,28,30;,
 4;16,15,31,33;,
 4;13,12,34,36;,
 4;37,39,8,7;,
 4;6,5,40,42;,
 4;2,1,46,48;,
 4;23,44,43,24;,
 4;49,50,51,52;,
 4;53,54,49,55;,
 4;56,57,58,59;,
 4;60,61,57,62;,
 4;63,64,62,65;,
 4;66,67,65,68;,
 4;69,70,68,71;,
 4;72,73,71,52;,
 3;74,75,76;,
 3;77,78,79;,
 3;80,81,82;,
 3;83,84,85;,
 3;86,87,88;,
 3;89,90,91;,
 3;92,93,94;,
 3;95,96,97;,
 4;70,69,74,76;,
 4;77,79,67,66;,
 4;80,82,64,63;,
 4;83,85,61,60;,
 4;59,58,86,88;,
 4;89,91,54,53;,
 4;95,97,51,50;,
 4;72,94,93,73;,
 4;4,55,49,3;,
 4;3,49,52,0;,
 4;0,52,71,20;,
 4;68,17,20,71;,
 4;17,68,65,14;,
 4;14,65,62,11;,
 4;11,62,57,9;,
 4;9,57,56,10;;
 
 MeshMaterialList {
  1;
  56;
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
   0.800000;0.138400;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  50;
  1.000000;0.000002;-0.000000;,
  0.273000;0.000002;-0.962014;,
  0.273001;-0.680245;-0.680248;,
  0.273002;-0.962014;-0.000000;,
  0.273001;-0.680245;0.680248;,
  0.273000;0.000002;0.962014;,
  0.272999;0.680246;0.680248;,
  0.273000;0.962014;0.000000;,
  0.272999;0.680246;-0.680248;,
  0.661983;0.749519;0.000000;,
  0.661983;0.529990;0.529990;,
  0.661983;0.000001;0.749519;,
  0.661984;-0.529988;0.529991;,
  0.661984;-0.749518;-0.000000;,
  0.661984;-0.529988;-0.529991;,
  0.661983;0.000001;-0.749519;,
  0.661983;0.529990;-0.529990;,
  0.899066;0.309582;0.309579;,
  0.899066;0.000001;0.437814;,
  0.899066;-0.309579;0.309582;,
  0.899066;-0.437813;-0.000000;,
  0.899066;-0.309579;-0.309582;,
  0.899066;0.000001;-0.437814;,
  0.899066;0.309582;-0.309579;,
  0.899065;0.437815;0.000000;,
  -1.000000;0.000001;0.000000;,
  -0.272994;0.000002;-0.962016;,
  -0.272996;-0.680247;-0.680248;,
  -0.272997;-0.962015;0.000000;,
  -0.272996;-0.680247;0.680248;,
  -0.272994;0.000002;0.962016;,
  -0.272994;0.680247;0.680249;,
  -0.272994;0.962016;0.000000;,
  -0.272994;0.680247;-0.680249;,
  -0.661980;0.749521;0.000000;,
  -0.661980;0.529992;0.529992;,
  -0.661979;0.000002;0.749522;,
  -0.661981;-0.529991;0.529991;,
  -0.661980;-0.749521;0.000000;,
  -0.661981;-0.529991;-0.529991;,
  -0.661979;0.000002;-0.749522;,
  -0.661980;0.529992;-0.529992;,
  -0.899066;0.309581;0.309579;,
  -0.899066;0.000001;0.437813;,
  -0.899067;-0.309581;0.309579;,
  -0.899065;-0.437815;0.000000;,
  -0.899067;-0.309581;-0.309579;,
  -0.899066;0.000001;-0.437813;,
  -0.899066;0.309581;-0.309579;,
  -0.899066;0.437814;0.000000;;
  56;
  4;7,9,16,8;,
  4;1,8,16,15;,
  4;15,14,2,1;,
  4;3,2,14,13;,
  4;4,3,13,12;,
  4;5,4,12,11;,
  4;6,5,11,10;,
  4;7,6,10,9;,
  3;18,0,17;,
  3;19,0,18;,
  3;20,0,19;,
  3;21,0,20;,
  3;22,0,21;,
  3;23,0,22;,
  3;24,17,0;,
  3;24,0,23;,
  4;18,17,10,11;,
  4;11,12,19,18;,
  4;12,13,20,19;,
  4;13,14,21,20;,
  4;22,21,14,15;,
  4;15,16,23,22;,
  4;16,9,24,23;,
  4;10,17,24,9;,
  4;33,41,34,32;,
  4;40,41,33,26;,
  4;26,27,39,40;,
  4;38,39,27,28;,
  4;37,38,28,29;,
  4;36,37,29,30;,
  4;35,36,30,31;,
  4;34,35,31,32;,
  3;42,25,43;,
  3;43,25,44;,
  3;44,25,45;,
  3;45,25,46;,
  3;46,25,47;,
  3;47,25,48;,
  3;25,42,49;,
  3;48,25,49;,
  4;36,35,42,43;,
  4;43,44,37,36;,
  4;44,45,38,37;,
  4;45,46,39,38;,
  4;40,39,46,47;,
  4;47,48,41,40;,
  4;48,49,34,41;,
  4;34,49,42,35;,
  4;1,26,33,8;,
  4;8,33,32,7;,
  4;7,32,31,6;,
  4;30,5,6,31;,
  4;5,30,29,4;,
  4;4,29,28,3;,
  4;3,28,27,2;,
  4;2,27,26,1;;
 }
 MeshTextureCoords {
  98;
  0.750000;0.000000;,
  0.781250;0.000000;,
  0.843750;0.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  0.906250;0.000000;,
  0.968750;0.000000;,
  0.031250;0.000000;,
  0.093750;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.156250;0.000000;,
  0.218750;0.000000;,
  0.375000;0.000000;,
  0.281250;0.000000;,
  0.343750;0.000000;,
  0.500000;0.000000;,
  0.406250;0.000000;,
  0.468750;0.000000;,
  0.625000;0.000000;,
  0.531250;0.000000;,
  0.593750;0.000000;,
  0.656250;0.000000;,
  0.718750;0.000000;,
  0.546880;0.000000;,
  0.562500;0.000000;,
  0.578120;0.000000;,
  0.421880;0.000000;,
  0.437500;0.000000;,
  0.453120;0.000000;,
  0.296880;0.000000;,
  0.312500;0.000000;,
  0.328120;0.000000;,
  0.171880;0.000000;,
  0.187500;0.000000;,
  0.203120;0.000000;,
  0.046880;0.000000;,
  0.062500;0.000000;,
  0.078120;0.000000;,
  0.921880;0.000000;,
  0.937500;0.000000;,
  0.953130;0.000000;,
  0.703120;0.000000;,
  0.671870;0.000000;,
  0.687500;0.000000;,
  0.796880;0.000000;,
  0.812500;0.000000;,
  0.828130;0.000000;,
  0.875000;0.000000;,
  0.843750;0.000000;,
  0.781250;0.000000;,
  0.750000;0.000000;,
  0.968750;0.000000;,
  0.906250;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.093750;0.000000;,
  0.031250;0.000000;,
  0.218750;0.000000;,
  0.156250;0.000000;,
  0.250000;0.000000;,
  0.343750;0.000000;,
  0.281250;0.000000;,
  0.375000;0.000000;,
  0.468750;0.000000;,
  0.406250;0.000000;,
  0.500000;0.000000;,
  0.593750;0.000000;,
  0.531250;0.000000;,
  0.625000;0.000000;,
  0.718750;0.000000;,
  0.656250;0.000000;,
  0.578120;0.000000;,
  0.562500;0.000000;,
  0.546880;0.000000;,
  0.453120;0.000000;,
  0.437500;0.000000;,
  0.421880;0.000000;,
  0.328120;0.000000;,
  0.312500;0.000000;,
  0.296880;0.000000;,
  0.203120;0.000000;,
  0.187500;0.000000;,
  0.171880;0.000000;,
  0.078120;0.000000;,
  0.062500;0.000000;,
  0.046880;0.000000;,
  0.953130;0.000000;,
  0.937500;0.000000;,
  0.921880;0.000000;,
  0.687500;0.000000;,
  0.671870;0.000000;,
  0.703120;0.000000;,
  0.828130;0.000000;,
  0.812500;0.000000;,
  0.796880;0.000000;;
 }
}
