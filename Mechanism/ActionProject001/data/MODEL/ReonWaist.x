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
 146;
 11.99388;-2.62608;-5.65539;,
 8.97402;0.00003;-4.24127;,
 8.97402;0.00003;3.49814;,
 11.99388;-2.62608;5.22932;,
 13.57784;-10.72712;4.56444;,
 13.57784;-10.72712;-4.61874;,
 13.48782;-12.70092;4.67450;,
 13.48782;-12.70092;-4.41368;,
 13.57784;-10.72712;-4.61874;,
 1.09421;-12.70092;-4.41368;,
 1.09421;-12.70092;4.67450;,
 0.00000;-10.72712;6.06680;,
 0.00000;-10.72712;-4.61874;,
 2.47793;-10.72712;-6.74561;,
 3.07665;-12.70092;-6.54054;,
 0.00000;-10.72712;-4.61874;,
 2.47793;-10.72712;-6.74561;,
 0.00000;-2.62608;-5.65539;,
 2.18886;-2.62608;-5.65539;,
 3.07665;-12.70092;6.48971;,
 0.00000;0.00003;-4.24127;,
 1.63775;0.00003;-4.24127;,
 2.47793;-10.72712;6.69066;,
 1.63775;0.00003;-4.24127;,
 0.00000;0.00003;-4.24127;,
 0.00000;0.00003;3.49814;,
 1.63775;0.00003;3.49814;,
 2.47793;-10.72712;6.69066;,
 2.18886;-2.62608;7.35554;,
 0.00000;-2.62608;6.73167;,
 1.63775;0.00003;3.49814;,
 10.12148;-10.72712;-6.74561;,
 9.19176;-12.70092;-6.54054;,
 10.12148;-10.72712;-6.74561;,
 8.94072;-2.62608;-5.65539;,
 11.99388;-2.62608;-5.65539;,
 13.57784;-10.72712;-4.61874;,
 9.19176;-12.70092;6.48971;,
 6.68960;0.00003;-4.24127;,
 8.97402;0.00003;-4.24127;,
 10.12148;-10.72712;6.69066;,
 6.68960;0.00003;-4.24127;,
 6.68960;0.00003;3.49814;,
 8.97402;0.00003;-4.24127;,
 10.12148;-10.72712;6.69066;,
 13.57784;-10.72712;4.56444;,
 11.99388;-2.62608;5.22932;,
 8.94072;-2.62608;7.35554;,
 6.68960;0.00003;3.49814;,
 8.97402;0.00003;3.49814;,
 -11.99388;-2.62608;5.22932;,
 -8.97402;0.00003;3.49814;,
 -8.97402;0.00003;-4.24127;,
 -11.99388;-2.62608;-5.65539;,
 -13.57784;-10.72712;-4.61874;,
 -13.57784;-10.72712;4.56444;,
 -13.57784;-10.72712;-4.61874;,
 -13.48782;-12.70092;-4.41368;,
 -13.48782;-12.70092;4.67450;,
 -1.09421;-12.70092;4.67450;,
 -1.09421;-12.70092;-4.41368;,
 -3.07665;-12.70092;-6.54054;,
 -2.47793;-10.72712;-6.74561;,
 -2.18886;-2.62608;-5.65539;,
 -2.47793;-10.72712;-6.74561;,
 -3.07665;-12.70092;6.48971;,
 -1.63775;0.00003;-4.24127;,
 -2.47793;-10.72712;6.69066;,
 -1.63775;0.00003;3.49814;,
 -1.63775;0.00003;-4.24127;,
 -2.18886;-2.62608;7.35554;,
 -2.47793;-10.72712;6.69066;,
 -1.63775;0.00003;3.49814;,
 -9.19176;-12.70092;-6.54054;,
 -10.12148;-10.72712;-6.74561;,
 -8.94072;-2.62608;-5.65539;,
 -10.12148;-10.72712;-6.74561;,
 -13.57784;-10.72712;-4.61874;,
 -11.99388;-2.62608;-5.65539;,
 -9.19176;-12.70092;6.48971;,
 -6.68960;0.00003;-4.24127;,
 -8.97402;0.00003;-4.24127;,
 -10.12148;-10.72712;6.69066;,
 -6.68960;0.00003;3.49814;,
 -6.68960;0.00003;-4.24127;,
 -8.97402;0.00003;-4.24127;,
 -8.94072;-2.62608;7.35554;,
 -11.99388;-2.62608;5.22932;,
 -13.57784;-10.72712;4.56444;,
 -10.12148;-10.72712;6.69066;,
 -6.68960;0.00003;3.49814;,
 -8.97402;0.00003;3.49814;,
 13.42925;0.61445;-7.89926;,
 13.42925;0.61445;6.82863;,
 13.42964;-2.62541;6.37892;,
 13.42964;-2.62541;-6.97068;,
 11.19702;0.61431;5.47808;,
 11.19702;0.61431;-6.68075;,
 11.99388;-2.62608;-5.65539;,
 11.99388;-2.62608;5.22932;,
 13.42925;0.61445;-7.89926;,
 11.19702;0.61431;-6.68075;,
 11.19702;0.61431;5.47808;,
 13.42925;0.61445;6.82863;,
 13.42964;-2.62541;-6.97068;,
 13.42964;-2.62541;6.37892;,
 11.99388;-2.62608;5.22932;,
 11.99388;-2.62608;-5.65539;,
 0.00039;0.61445;-7.89926;,
 0.00078;-2.62541;-6.97068;,
 0.00039;0.61445;-7.89926;,
 0.00000;0.61431;-6.68075;,
 0.00078;-2.62541;-6.97068;,
 0.00000;-2.62608;-5.65539;,
 0.00000;0.61431;-6.68075;,
 0.00000;-2.62608;-5.65539;,
 0.00039;0.61445;6.82862;,
 0.00000;-2.62608;6.73167;,
 0.00039;0.61445;6.82862;,
 0.00000;0.61431;5.26083;,
 0.00000;-2.62608;6.73167;,
 0.00000;-2.62608;5.22932;,
 0.00000;0.61431;5.26083;,
 0.00000;-2.62608;5.22932;,
 -13.42964;-2.62541;-6.97068;,
 -13.42964;-2.62541;6.37892;,
 -13.42925;0.61445;6.82863;,
 -13.42925;0.61445;-7.89926;,
 -11.99388;-2.62608;5.22932;,
 -11.99388;-2.62608;-5.65539;,
 -11.19702;0.61431;-6.68075;,
 -11.19702;0.61431;5.47808;,
 -13.42925;0.61445;6.82863;,
 -11.19702;0.61431;5.47808;,
 -11.19702;0.61431;-6.68075;,
 -13.42925;0.61445;-7.89926;,
 -11.99388;-2.62608;-5.65539;,
 -11.99388;-2.62608;5.22932;,
 -13.42964;-2.62541;6.37892;,
 -13.42964;-2.62541;-6.97068;,
 -0.00078;-2.62541;-6.97068;,
 -0.00039;0.61445;-7.89926;,
 -0.00039;0.61445;-7.89926;,
 -0.00078;-2.62541;-6.97068;,
 -0.00039;0.61445;6.82862;,
 -0.00039;0.61445;6.82862;;
 
 80;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;6,7,8,4;,
 4;9,10,11,12;,
 4;13,14,9,15;,
 4;16,12,17,18;,
 4;14,19,10,9;,
 4;18,17,20,21;,
 4;19,22,11,10;,
 4;23,24,25,26;,
 4;27,28,29,11;,
 4;30,25,29,28;,
 4;31,8,7,32;,
 4;31,32,14,13;,
 4;33,16,18,34;,
 4;33,34,35,36;,
 4;32,7,6,37;,
 4;32,37,19,14;,
 4;34,18,21,38;,
 4;34,38,39,35;,
 4;37,6,4,40;,
 4;37,40,22,19;,
 4;41,23,26,42;,
 4;41,42,2,43;,
 4;44,45,46,47;,
 4;44,47,28,27;,
 4;48,30,28,47;,
 4;48,47,46,49;,
 4;50,51,52,53;,
 4;54,55,50,53;,
 4;55,56,57,58;,
 4;12,11,59,60;,
 4;15,60,61,62;,
 4;63,17,12,64;,
 4;60,59,65,61;,
 4;66,20,17,63;,
 4;59,11,67,65;,
 4;68,25,24,69;,
 4;11,29,70,71;,
 4;70,29,25,72;,
 4;73,57,56,74;,
 4;62,61,73,74;,
 4;75,63,64,76;,
 4;77,78,75,76;,
 4;79,58,57,73;,
 4;61,65,79,73;,
 4;80,66,63,75;,
 4;78,81,80,75;,
 4;82,55,58,79;,
 4;65,67,82,79;,
 4;83,68,69,84;,
 4;85,51,83,84;,
 4;86,87,88,89;,
 4;71,70,86,89;,
 4;86,70,72,90;,
 4;91,87,86,90;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;108,92,95,109;,
 4;110,111,101,100;,
 4;112,104,107,113;,
 4;114,115,98,97;,
 4;116,117,94,93;,
 4;118,103,102,119;,
 4;120,121,106,105;,
 4;122,96,99,123;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;140,124,127,141;,
 4;135,134,111,142;,
 4;113,136,139,143;,
 4;130,129,115,114;,
 4;126,125,117,144;,
 4;119,133,132,145;,
 4;138,137,121,120;,
 4;123,128,131,122;;
 
 MeshMaterialList {
  3;
  80;
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
  1,
  1,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  0,
  1,
  1,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
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
   0.276000;0.276000;0.276000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  94;
  0.000000;0.474117;-0.880462;,
  -0.716249;-0.326297;-0.616861;,
  0.350295;0.936640;0.000000;,
  0.997281;0.073691;0.000000;,
  0.305607;0.930713;0.200943;,
  0.000000;0.947851;0.318714;,
  0.947466;0.063927;0.313404;,
  -0.410877;-0.351940;0.841022;,
  0.865796;0.500397;0.000000;,
  0.000000;0.239433;-0.970913;,
  0.703409;0.677916;0.213649;,
  -0.000000;0.412862;0.910794;,
  -0.818949;-0.444279;-0.363233;,
  0.998962;-0.045557;-0.000000;,
  -0.731358;-0.533073;0.425381;,
  -0.266421;-0.090328;-0.959615;,
  -0.352358;-0.241441;-0.904185;,
  -0.090326;0.274003;-0.957478;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.474117;-0.880462;,
  -0.180909;-0.191884;0.964599;,
  -0.021475;0.951639;0.306466;,
  -0.098146;0.419935;0.902231;,
  0.193004;-0.027179;-0.980821;,
  0.245537;-0.157494;-0.956508;,
  0.070083;0.294677;-0.953023;,
  0.000000;0.474117;-0.880462;,
  0.260423;-0.073253;0.962712;,
  0.048912;0.954828;0.293105;,
  0.214286;0.461313;0.860971;,
  -0.350295;0.936640;0.000000;,
  -0.997281;0.073691;0.000000;,
  -0.305607;0.930713;0.200943;,
  -0.947466;0.063927;0.313404;,
  -0.865796;0.500397;0.000000;,
  -0.703409;0.677916;0.213649;,
  0.818949;-0.444279;-0.363233;,
  -0.998962;-0.045557;0.000000;,
  0.731358;-0.533073;0.425381;,
  0.266421;-0.090328;-0.959615;,
  0.352358;-0.241441;-0.904185;,
  0.090326;0.274003;-0.957478;,
  0.180909;-0.191884;0.964599;,
  0.021475;0.951639;0.306466;,
  0.098146;0.419935;0.902231;,
  -0.193004;-0.027179;-0.980821;,
  -0.245537;-0.157494;-0.956508;,
  -0.070083;0.294677;-0.953023;,
  -0.260423;-0.073253;0.962712;,
  -0.048912;0.954828;0.293105;,
  -0.214286;0.461313;0.860971;,
  1.000000;0.000120;0.000000;,
  -0.971069;-0.238800;0.000000;,
  0.000000;-0.275517;-0.961296;,
  0.000000;1.000000;0.000113;,
  0.000000;-1.000000;-0.000512;,
  0.013027;-0.083800;0.996397;,
  0.000000;-0.083808;0.996482;,
  0.000000;1.000000;-0.000096;,
  0.000000;-1.000000;0.000253;,
  -1.000000;0.000120;0.000000;,
  0.971069;-0.238800;-0.000000;,
  -0.013027;-0.083800;0.996397;,
  -0.230896;-0.301827;0.924980;,
  0.000000;1.000000;0.000000;,
  0.379966;-0.068762;-0.922441;,
  0.475193;-0.201464;-0.856507;,
  0.140901;0.278852;-0.949941;,
  0.000000;0.474117;-0.880462;,
  0.234381;-0.115583;0.965249;,
  0.455633;-0.123405;0.881572;,
  0.547784;0.024244;0.836269;,
  0.716249;-0.326297;-0.616861;,
  0.230896;-0.301827;0.924980;,
  -0.475193;-0.201464;-0.856507;,
  -0.379966;-0.068762;-0.922441;,
  -0.140901;0.278852;-0.949941;,
  -0.455633;-0.123405;0.881572;,
  -0.234381;-0.115583;0.965249;,
  -0.547784;0.024244;0.836269;,
  -0.000031;1.000000;0.000056;,
  -0.000030;1.000000;-0.000048;,
  0.000234;-1.000000;-0.000256;,
  0.000250;-1.000000;0.000127;,
  0.000000;0.301686;0.953407;,
  0.000001;1.000000;-0.000096;,
  0.000000;0.043162;-0.999068;,
  0.009705;0.043160;-0.999021;,
  0.000030;1.000000;-0.000048;,
  0.000031;1.000000;0.000056;,
  -0.000234;-1.000000;-0.000256;,
  -0.000250;-1.000000;0.000127;,
  -0.000001;1.000000;-0.000096;,
  -0.009705;0.043160;-0.999021;;
  80;
  4;8,2,4,10;,
  4;8,10,6,3;,
  4;13,13,3,6;,
  4;12,14,7,1;,
  4;15,16,12,1;,
  4;15,1,9,17;,
  4;18,18,18,18;,
  4;17,9,0,19;,
  4;63,20,7,14;,
  4;64,64,5,21;,
  4;20,22,11,7;,
  4;21,5,11,22;,
  4;23,65,66,24;,
  4;23,24,16,15;,
  4;23,15,17,25;,
  4;23,25,67,65;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;25,17,19,26;,
  4;25,26,68,67;,
  4;69,70,70,27;,
  4;69,27,20,63;,
  4;64,64,21,28;,
  4;64,28,4,2;,
  4;27,6,71,29;,
  4;27,29,22,20;,
  4;28,21,22,29;,
  4;28,29,10,4;,
  4;35,32,30,34;,
  4;31,33,35,34;,
  4;33,31,37,37;,
  4;72,38,38,36;,
  4;72,36,40,39;,
  4;41,9,72,39;,
  4;18,18,18,18;,
  4;19,0,9,41;,
  4;38,38,42,73;,
  4;43,5,64,64;,
  4;7,11,44,42;,
  4;44,11,5,43;,
  4;46,74,75,45;,
  4;39,40,46,45;,
  4;47,41,39,45;,
  4;75,76,47,45;,
  4;18,18,18,18;,
  4;18,18,18,18;,
  4;26,19,41,47;,
  4;76,68,26,47;,
  4;48,77,77,78;,
  4;73,42,48,78;,
  4;49,43,64,64;,
  4;30,32,49,64;,
  4;50,79,33,48;,
  4;42,44,50,48;,
  4;50,44,43,49;,
  4;32,35,50,49;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;80,80,81,81;,
  4;82,83,83,82;,
  4;53,53,53,53;,
  4;54,54,80,80;,
  4;55,82,82,55;,
  4;84,84,84,84;,
  4;56,57,56,56;,
  4;85,81,81,58;,
  4;59,59,83,83;,
  4;86,87,87,86;,
  4;60,60,60,60;,
  4;61,61,61,61;,
  4;88,88,89,89;,
  4;90,91,91,90;,
  4;53,53,53,53;,
  4;89,89,54,54;,
  4;55,90,90,55;,
  4;84,84,84,84;,
  4;62,62,57,62;,
  4;58,88,88,92;,
  4;91,91,59,59;,
  4;86,93,93,86;;
 }
 MeshTextureCoords {
  146;
  0.000000;0.054750;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.054750;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.591250;0.000000;,
  0.591250;0.000000;,
  0.500000;0.000000;,
  0.591250;1.000000;,
  0.500000;0.054750;,
  0.591250;0.054750;,
  0.591250;1.000000;,
  0.500000;0.000000;,
  0.591250;0.000000;,
  0.591250;1.000000;,
  0.591250;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.591250;0.000000;,
  0.408750;1.000000;,
  0.408750;0.054750;,
  0.500000;0.054750;,
  0.408750;0.000000;,
  0.872720;0.000000;,
  0.872720;0.000000;,
  0.872720;1.000000;,
  0.872720;0.054750;,
  1.000000;0.054750;,
  1.000000;1.000000;,
  0.872720;1.000000;,
  0.872720;0.000000;,
  1.000000;0.000000;,
  0.872720;1.000000;,
  0.872720;1.000000;,
  0.872720;0.000000;,
  1.000000;1.000000;,
  0.127280;1.000000;,
  0.000000;1.000000;,
  0.000000;0.054750;,
  0.127280;0.054750;,
  0.127280;0.000000;,
  0.000000;0.000000;,
  1.000000;0.054750;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.054750;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.591250;0.000000;,
  0.591250;0.000000;,
  0.591250;0.054750;,
  0.591250;1.000000;,
  0.591250;1.000000;,
  0.591250;0.000000;,
  0.591250;1.000000;,
  0.591250;0.000000;,
  0.591250;1.000000;,
  0.408750;0.054750;,
  0.408750;1.000000;,
  0.408750;0.000000;,
  0.872720;0.000000;,
  0.872720;0.000000;,
  0.872720;0.054750;,
  0.872720;1.000000;,
  1.000000;1.000000;,
  1.000000;0.054750;,
  0.872720;1.000000;,
  0.872720;0.000000;,
  1.000000;0.000000;,
  0.872720;1.000000;,
  0.872720;0.000000;,
  0.872720;1.000000;,
  1.000000;1.000000;,
  0.127280;0.054750;,
  0.000000;0.054750;,
  0.000000;1.000000;,
  0.127280;1.000000;,
  0.127280;0.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.250000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.625000;0.000000;,
  0.000000;0.000000;;
 }
}