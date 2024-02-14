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
 90;
 0.00000;98.56409;-19.13415;,
 13.52990;98.56409;-13.52990;,
 25.00000;90.25385;-25.00000;,
 0.00000;90.25385;-35.35535;,
 19.13415;98.56409;0.00000;,
 35.35535;90.25385;0.00000;,
 13.52990;98.56409;13.52990;,
 25.00000;90.25385;25.00000;,
 0.00000;98.56409;19.13415;,
 0.00000;90.25385;35.35535;,
 -13.52990;98.56409;13.52990;,
 -25.00000;90.25385;25.00000;,
 -19.13415;98.56409;0.00000;,
 -35.35535;90.25385;0.00000;,
 -13.52990;98.56409;-13.52990;,
 -25.00000;90.25385;-25.00000;,
 0.00000;98.56409;-19.13415;,
 0.00000;90.25385;-35.35535;,
 32.66405;76.08265;-32.66405;,
 0.00000;76.08265;-46.19395;,
 46.19395;76.08265;0.00000;,
 32.66405;76.08265;32.66405;,
 0.00000;76.08265;46.19395;,
 -32.66410;76.08265;32.66405;,
 -46.19395;76.08265;0.00000;,
 -32.66410;76.08265;-32.66405;,
 0.00000;76.08265;-46.19395;,
 35.35535;61.21000;-35.35535;,
 0.00000;61.21000;-50.00000;,
 50.00000;61.21000;0.00000;,
 35.35535;61.21000;35.35535;,
 0.00000;61.21000;50.00000;,
 -35.35535;61.21000;35.35535;,
 -50.00000;61.21000;0.00000;,
 -35.35535;61.21000;-35.35535;,
 0.00000;61.21000;-50.00000;,
 32.66405;42.07580;-32.66405;,
 0.00000;42.07580;-46.19395;,
 46.19395;42.07580;0.00000;,
 32.66405;42.07580;32.66405;,
 0.00000;42.07580;46.19395;,
 -32.66410;42.07580;32.66405;,
 -46.19395;42.07580;0.00000;,
 -32.66410;42.07580;-32.66405;,
 0.00000;42.07580;-46.19395;,
 25.00000;19.03545;-25.00000;,
 0.00000;19.03545;-35.35535;,
 35.35535;19.03545;0.00000;,
 25.00000;19.03545;25.00000;,
 0.00000;19.03545;35.35535;,
 -25.00000;19.03545;25.00000;,
 -35.35535;19.03545;0.00000;,
 -25.00000;19.03545;-25.00000;,
 0.00000;19.03545;-35.35535;,
 13.52990;3.80489;-13.52990;,
 0.00000;3.80489;-19.13415;,
 19.13415;3.80489;0.00000;,
 13.52990;3.80489;13.52990;,
 0.00000;3.80489;19.13415;,
 -13.52990;3.80489;13.52990;,
 -19.13415;3.80489;0.00000;,
 -13.52990;3.80489;-13.52990;,
 0.00000;3.80489;-19.13415;,
 -2.77715;102.20415;2.77540;,
 -3.91895;102.21835;-0.00280;,
 0.00105;102.20415;3.92620;,
 2.76935;102.21835;2.76760;,
 3.91690;102.21835;-0.00280;,
 2.76935;102.21835;-2.77315;,
 -0.00105;102.21835;-3.92070;,
 -13.52990;98.56409;-13.52990;,
 -2.77715;102.20415;-2.78100;,
 -2.77715;102.20415;-2.78100;,
 0.00000;0.45940;-4.46040;,
 0.00000;-0.00111;-0.00000;,
 -3.15395;0.45940;-3.15395;,
 3.15395;0.45940;-3.15395;,
 4.46040;0.45940;0.00000;,
 3.15395;0.45940;3.15395;,
 0.00000;-0.00111;-0.00000;,
 0.00000;0.45940;4.46040;,
 -3.15395;0.45940;3.15395;,
 -4.46040;0.45940;0.00000;,
 -3.15395;0.45940;-3.15395;,
 0.00000;0.45940;-4.46040;,
 -0.00105;102.62810;-0.00280;,
 3.91690;102.21835;-0.00280;,
 2.76935;102.21835;-2.77315;,
 -0.00105;102.21835;-3.92070;,
 -0.00105;102.62810;-0.00280;;
 
 80;
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
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;37,36,45,46;,
 4;36,38,47,45;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;46,45,54,55;,
 4;45,47,56,54;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 4;12,10,63,64;,
 4;10,8,65,63;,
 4;66,65,8,6;,
 4;67,66,6,4;,
 4;68,67,4,1;,
 4;1,0,69,68;,
 4;0,70,71,69;,
 4;12,64,72,14;,
 3;73,74,75;,
 3;76,74,73;,
 3;77,74,76;,
 3;78,79,77;,
 3;80,79,78;,
 3;81,79,80;,
 3;82,83,79;,
 3;82,79,81;,
 4;61,62,84,83;,
 4;55,54,76,73;,
 4;56,57,78,77;,
 4;54,56,77,76;,
 4;57,58,80,78;,
 4;81,80,58,59;,
 4;59,60,82,81;,
 4;82,60,61,83;,
 3;85,72,64;,
 3;85,86,87;,
 3;88,85,87;,
 3;72,85,88;,
 3;64,63,85;,
 3;65,85,63;,
 3;65,66,85;,
 3;66,67,89;;
 
 MeshMaterialList {
  1;
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
    "data\\TEXTURE\\ParmFruit.png";
   }
  }
 }
 MeshNormals {
  74;
  -0.000481;0.938085;-0.346403;,
  0.244992;0.938060;-0.244996;,
  0.346458;0.938066;-0.000004;,
  0.244593;0.938099;0.245245;,
  -0.000323;0.938048;0.346506;,
  -0.244793;0.938038;0.245278;,
  -0.346306;0.938122;-0.000004;,
  -0.245341;0.937874;-0.245357;,
  0.000000;0.770079;-0.637949;,
  0.451098;0.770079;-0.451098;,
  0.637949;0.770079;0.000000;,
  0.451098;0.770079;0.451098;,
  0.000000;0.770079;0.637949;,
  -0.451098;0.770079;0.451098;,
  -0.637948;0.770079;0.000000;,
  -0.451098;0.770079;-0.451098;,
  0.000000;0.438656;-0.898655;,
  0.635445;0.438656;-0.635445;,
  0.898655;0.438656;0.000000;,
  0.635445;0.438656;0.635445;,
  0.000000;0.438656;0.898655;,
  -0.635445;0.438656;0.635445;,
  -0.898655;0.438655;0.000000;,
  -0.635445;0.438656;-0.635445;,
  0.000000;0.027284;-0.999628;,
  0.706844;0.027284;-0.706844;,
  0.999628;0.027284;0.000000;,
  0.706844;0.027284;0.706844;,
  0.000000;0.027284;0.999628;,
  -0.706844;0.027284;0.706843;,
  -0.999628;0.027284;0.000000;,
  -0.706844;0.027284;-0.706843;,
  0.000000;-0.313312;-0.949650;,
  0.671504;-0.313312;-0.671504;,
  0.949650;-0.313312;-0.000000;,
  0.671504;-0.313312;0.671504;,
  0.000000;-0.313312;0.949650;,
  -0.671504;-0.313312;0.671504;,
  -0.949650;-0.313312;0.000000;,
  -0.671504;-0.313312;-0.671504;,
  0.000000;-0.589853;-0.807511;,
  0.570996;-0.589853;-0.570996;,
  0.807511;-0.589853;-0.000000;,
  0.570996;-0.589853;0.570996;,
  0.000000;-0.589853;0.807511;,
  -0.570996;-0.589853;0.570996;,
  -0.807510;-0.589853;0.000000;,
  -0.570996;-0.589853;-0.570996;,
  0.000000;-0.884971;-0.465647;,
  0.329262;-0.884970;-0.329262;,
  0.465647;-0.884971;-0.000000;,
  0.329262;-0.884970;0.329262;,
  0.000000;-0.884971;0.465647;,
  -0.329262;-0.884970;0.329262;,
  -0.465647;-0.884971;0.000000;,
  -0.329262;-0.884970;-0.329262;,
  0.000000;-1.000000;-0.000000;,
  -0.119645;0.985317;0.121800;,
  -0.001486;0.985325;0.170681;,
  -0.121038;0.985238;-0.121065;,
  -0.115041;-0.986677;-0.115041;,
  0.000000;-0.986677;-0.162692;,
  0.115041;-0.986677;-0.115041;,
  0.162692;-0.986677;-0.000000;,
  0.115041;-0.986677;0.115041;,
  0.000000;-0.986677;0.162692;,
  -0.115041;-0.986677;0.115041;,
  -0.162692;-0.986677;0.000000;,
  -0.001653;0.985625;-0.168941;,
  0.119512;0.985613;-0.119518;,
  0.169002;0.985616;-0.000004;,
  0.118290;0.985631;0.120576;,
  -0.168835;0.985644;-0.000004;,
  -0.001169;0.999999;0.000824;;
  80;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  4;6,5,57,72;,
  4;5,4,58,57;,
  4;71,58,4,3;,
  4;70,71,3,2;,
  4;69,70,2,1;,
  4;1,0,68,69;,
  4;0,7,59,68;,
  4;6,72,59,7;,
  3;61,56,60;,
  3;62,56,61;,
  3;63,56,62;,
  3;64,56,63;,
  3;65,56,64;,
  3;66,56,65;,
  3;67,60,56;,
  3;67,56,66;,
  4;55,48,61,60;,
  4;48,49,62,61;,
  4;50,51,64,63;,
  4;49,50,63,62;,
  4;51,52,65,64;,
  4;66,65,52,53;,
  4;53,54,67,66;,
  4;67,54,55,60;,
  3;73,59,72;,
  3;73,70,69;,
  3;68,73,69;,
  3;59,73,68;,
  3;72,57,73;,
  3;58,73,57;,
  3;58,71,73;,
  3;71,70,73;;
 }
 MeshTextureCoords {
  90;
  0.000000;0.136230;,
  0.125000;0.136230;,
  0.125000;0.254240;,
  0.000000;0.254240;,
  0.250000;0.136230;,
  0.250000;0.254240;,
  0.375000;0.136230;,
  0.375000;0.254240;,
  0.500000;0.136230;,
  0.500000;0.254240;,
  0.625000;0.136230;,
  0.625000;0.254240;,
  0.750000;0.136230;,
  0.750000;0.254240;,
  0.875000;0.136230;,
  0.875000;0.254240;,
  1.000000;0.136230;,
  1.000000;0.254240;,
  0.125000;0.360780;,
  0.000000;0.360780;,
  0.250000;0.360780;,
  0.375000;0.360780;,
  0.500000;0.360780;,
  0.625000;0.360780;,
  0.750000;0.360780;,
  0.875000;0.360780;,
  1.000000;0.360780;,
  0.125000;0.448640;,
  0.000000;0.448640;,
  0.250000;0.448640;,
  0.375000;0.448640;,
  0.500000;0.448640;,
  0.625000;0.448640;,
  0.750000;0.448640;,
  0.875000;0.448640;,
  1.000000;0.448640;,
  0.125000;0.562440;,
  0.000000;0.562440;,
  0.250000;0.562440;,
  0.375000;0.562440;,
  0.500000;0.562440;,
  0.625000;0.562440;,
  0.750000;0.562440;,
  0.875000;0.562440;,
  1.000000;0.562440;,
  0.125000;0.724140;,
  0.000000;0.724140;,
  0.250000;0.724140;,
  0.375000;0.724140;,
  0.500000;0.724140;,
  0.625000;0.724140;,
  0.750000;0.724140;,
  0.875000;0.724140;,
  1.000000;0.724140;,
  0.125000;0.868940;,
  0.000000;0.868940;,
  0.250000;0.868940;,
  0.375000;0.868940;,
  0.500000;0.868940;,
  0.625000;0.868940;,
  0.750000;0.868940;,
  0.875000;0.868940;,
  1.000000;0.868940;,
  0.625050;0.027560;,
  0.750110;0.027500;,
  0.499960;0.027560;,
  0.374950;0.027470;,
  0.249890;0.027480;,
  0.124890;0.027500;,
  -0.000040;0.027510;,
  -0.125000;0.136230;,
  -0.124890;0.027580;,
  0.875110;0.027580;,
  0.000000;0.969780;,
  -0.250000;1.000000;,
  -0.125000;0.969780;,
  0.125000;0.969780;,
  0.250000;0.969780;,
  0.375000;0.969780;,
  0.750000;1.000000;,
  0.500000;0.969780;,
  0.625000;0.969780;,
  0.750000;0.969780;,
  0.875000;0.969780;,
  1.000000;0.969780;,
  0.943700;0.000020;,
  1.249890;0.027480;,
  1.124890;0.027500;,
  0.999960;0.027510;,
  -0.056300;0.000020;;
 }
}