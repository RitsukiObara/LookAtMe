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
 62;
 0.00000;2.94064;-13.15629;,
 2.73535;3.90613;-12.86880;,
 2.07912;0.00000;-9.78148;,
 0.00000;0.00000;-10.00000;,
 5.35115;2.51197;-12.01887;,
 4.06737;0.00000;-9.13545;,
 7.73307;2.50710;-10.64366;,
 5.87785;0.00000;-8.09017;,
 9.77703;1.44744;-8.80328;,
 7.43145;0.00000;-6.69131;,
 11.39368;3.03961;-6.57814;,
 8.66025;0.00000;-5.00000;,
 12.51238;2.55762;-4.06552;,
 9.51057;0.00000;-3.09017;,
 13.08422;2.34162;-1.37520;,
 9.94522;0.00000;-1.04528;,
 13.08422;3.96655;1.37521;,
 9.94522;0.00000;1.04529;,
 12.51238;3.56621;4.06552;,
 9.51057;0.00000;3.09017;,
 11.39368;4.38253;6.57814;,
 8.66025;0.00000;5.00000;,
 9.77703;2.63641;8.80328;,
 7.43145;0.00000;6.69131;,
 7.73307;1.41230;10.64366;,
 5.87785;0.00000;8.09017;,
 5.35115;0.76006;12.01887;,
 4.06737;0.00000;9.13545;,
 2.73535;2.86169;12.86880;,
 2.07912;0.00000;9.78148;,
 0.00000;3.79665;13.15629;,
 0.00000;0.00000;10.00000;,
 -2.73535;5.18101;12.86880;,
 -2.07912;0.00000;9.78148;,
 -5.35115;4.57339;12.01887;,
 -4.06737;0.00000;9.13545;,
 -7.73307;3.23531;10.64366;,
 -5.87785;0.00000;8.09017;,
 -9.77703;3.30909;8.80328;,
 -7.43145;0.00000;6.69131;,
 -11.39368;1.39693;6.57814;,
 -8.66025;0.00000;5.00000;,
 -12.51238;3.41963;4.06552;,
 -9.51057;0.00000;3.09017;,
 -13.08422;3.90891;1.37521;,
 -9.94522;0.00000;1.04529;,
 -13.08422;2.34214;-1.37520;,
 -9.94522;0.00000;-1.04528;,
 -12.51238;2.28001;-4.06550;,
 -9.51057;0.00000;-3.09016;,
 -11.39369;3.86138;-6.57813;,
 -8.66026;0.00000;-4.99999;,
 -9.77704;2.95402;-8.80327;,
 -7.43146;0.00000;-6.69130;,
 -7.73308;4.21979;-10.64365;,
 -5.87786;0.00000;-8.09016;,
 -5.35116;2.80569;-12.01887;,
 -4.06738;0.00000;-9.13545;,
 -2.73538;1.61786;-12.86878;,
 -2.07914;0.00000;-9.78147;,
 0.00000;2.94064;-13.15629;,
 0.00000;0.00000;-10.00000;;
 
 60;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;50,52,53,51;,
 4;52,54,55,53;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 4;58,60,61,59;,
 4;3,2,1,0;,
 4;2,5,4,1;,
 4;5,7,6,4;,
 4;7,9,8,6;,
 4;9,11,10,8;,
 4;11,13,12,10;,
 4;13,15,14,12;,
 4;15,17,16,14;,
 4;17,19,18,16;,
 4;19,21,20,18;,
 4;21,23,22,20;,
 4;23,25,24,22;,
 4;25,27,26,24;,
 4;27,29,28,26;,
 4;29,31,30,28;,
 4;31,33,32,30;,
 4;33,35,34,32;,
 4;35,37,36,34;,
 4;37,39,38,36;,
 4;39,41,40,38;,
 4;41,43,42,40;,
 4;43,45,44,42;,
 4;45,47,46,44;,
 4;47,49,48,46;,
 4;49,51,50,48;,
 4;51,53,52,50;,
 4;53,55,54,52;,
 4;55,57,56,54;,
 4;57,59,58,56;,
 4;59,61,60,58;;
 
 MeshMaterialList {
  1;
  60;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  60;
  0.150560;-0.752101;-0.641620;,
  0.124196;-0.696227;-0.706996;,
  0.189410;-0.753490;-0.629584;,
  0.270382;-0.816740;-0.509734;,
  0.401009;-0.857692;-0.321802;,
  0.567026;-0.788270;-0.238980;,
  0.586966;-0.772449;-0.242475;,
  0.643648;-0.765227;0.012022;,
  0.717279;-0.680867;0.148092;,
  0.723577;-0.640361;0.257631;,
  0.679742;-0.653259;0.333472;,
  0.579559;-0.768658;0.270697;,
  0.360114;-0.900750;0.242832;,
  0.062830;-0.931622;0.357956;,
  -0.081001;-0.789494;0.608390;,
  -0.124207;-0.640232;0.758073;,
  -0.208843;-0.562726;0.799828;,
  -0.233103;-0.590881;0.772349;,
  -0.379790;-0.671786;0.635974;,
  -0.397143;-0.756003;0.520323;,
  -0.467178;-0.843699;0.264417;,
  -0.676894;-0.734224;0.052253;,
  -0.712624;-0.687538;0.139495;,
  -0.634752;-0.772079;0.031371;,
  -0.565741;-0.776311;-0.277988;,
  -0.592099;-0.706087;-0.388407;,
  -0.527664;-0.688051;-0.498154;,
  -0.441727;-0.672540;-0.593774;,
  -0.409626;-0.754194;-0.513224;,
  -0.105723;-0.839503;-0.532970;,
  -0.150560;0.752101;0.641620;,
  -0.124196;0.696227;0.706996;,
  -0.189410;0.753490;0.629584;,
  -0.270382;0.816740;0.509734;,
  -0.401009;0.857692;0.321802;,
  -0.567026;0.788270;0.238980;,
  -0.586966;0.772449;0.242475;,
  -0.643648;0.765227;-0.012022;,
  -0.717279;0.680867;-0.148092;,
  -0.723577;0.640361;-0.257631;,
  -0.679742;0.653259;-0.333472;,
  -0.579559;0.768658;-0.270697;,
  -0.360114;0.900750;-0.242832;,
  -0.062830;0.931622;-0.357956;,
  0.081001;0.789494;-0.608390;,
  0.124207;0.640232;-0.758073;,
  0.208843;0.562726;-0.799828;,
  0.233103;0.590881;-0.772349;,
  0.379790;0.671786;-0.635974;,
  0.397143;0.756003;-0.520323;,
  0.467178;0.843699;-0.264417;,
  0.676894;0.734224;-0.052253;,
  0.712624;0.687538;-0.139495;,
  0.634752;0.772079;-0.031371;,
  0.565741;0.776311;0.277988;,
  0.592099;0.706087;0.388407;,
  0.527664;0.688051;0.498154;,
  0.441727;0.672540;0.593774;,
  0.409626;0.754194;0.513224;,
  0.105723;0.839503;0.532970;;
  60;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,19,19,18;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,25,25,24;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,0,0,29;,
  4;30,31,31,30;,
  4;31,32,32,31;,
  4;32,33,33,32;,
  4;33,34,34,33;,
  4;34,35,35,34;,
  4;35,36,36,35;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;39,40,40,39;,
  4;40,41,41,40;,
  4;41,42,42,41;,
  4;42,43,43,42;,
  4;43,44,44,43;,
  4;44,45,45,44;,
  4;45,46,46,45;,
  4;46,47,47,46;,
  4;47,48,48,47;,
  4;48,49,49,48;,
  4;49,50,50,49;,
  4;50,51,51,50;,
  4;51,52,52,51;,
  4;52,53,53,52;,
  4;53,54,54,53;,
  4;54,55,55,54;,
  4;55,56,56,55;,
  4;56,57,57,56;,
  4;57,58,58,57;,
  4;58,59,59,58;,
  4;59,30,30,59;;
 }
 MeshTextureCoords {
  62;
  0.000000;0.000000;,
  0.033330;0.000000;,
  0.033330;1.000000;,
  0.000000;1.000000;,
  0.066670;0.000000;,
  0.066670;1.000000;,
  0.100000;0.000000;,
  0.100000;1.000000;,
  0.133330;0.000000;,
  0.133330;1.000000;,
  0.166670;0.000000;,
  0.166670;1.000000;,
  0.200000;0.000000;,
  0.200000;1.000000;,
  0.233330;0.000000;,
  0.233330;1.000000;,
  0.266670;0.000000;,
  0.266670;1.000000;,
  0.300000;0.000000;,
  0.300000;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.366670;0.000000;,
  0.366670;1.000000;,
  0.400000;0.000000;,
  0.400000;1.000000;,
  0.433330;0.000000;,
  0.433330;1.000000;,
  0.466670;0.000000;,
  0.466670;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.533330;0.000000;,
  0.533330;1.000000;,
  0.566670;0.000000;,
  0.566670;1.000000;,
  0.600000;0.000000;,
  0.600000;1.000000;,
  0.633330;0.000000;,
  0.633330;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.700000;0.000000;,
  0.700000;1.000000;,
  0.733330;0.000000;,
  0.733330;1.000000;,
  0.766670;0.000000;,
  0.766670;1.000000;,
  0.800000;0.000000;,
  0.800000;1.000000;,
  0.833330;0.000000;,
  0.833330;1.000000;,
  0.866670;0.000000;,
  0.866670;1.000000;,
  0.900000;0.000000;,
  0.900000;1.000000;,
  0.933330;0.000000;,
  0.933330;1.000000;,
  0.966670;0.000000;,
  0.966670;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;;
 }
}