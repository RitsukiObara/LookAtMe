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
 141;
 0.00000;49.39620;-209.70932;,
 -13.06410;47.26240;-190.04431;,
 0.00000;47.26240;-181.89902;,
 0.00000;49.39620;-209.70932;,
 -18.47540;47.26240;-209.70932;,
 0.00000;49.39620;-209.70932;,
 -13.06410;47.26240;-229.37430;,
 0.00000;49.39620;-209.70932;,
 0.00000;47.26240;-237.51990;,
 0.00000;49.39620;-209.70932;,
 13.06410;47.26240;-229.37430;,
 0.00000;49.39620;-209.70932;,
 18.47540;47.26240;-209.70932;,
 0.00000;49.39620;-209.70932;,
 13.06410;47.26240;-190.04431;,
 0.00000;49.39620;-209.70932;,
 0.00000;47.26240;-181.89902;,
 -24.13930;41.18590;-173.37320;,
 0.00000;41.18590;-158.32202;,
 -34.13820;41.18590;-209.70932;,
 -24.13930;41.18590;-246.04572;,
 0.00000;41.18590;-261.09641;,
 24.13940;41.18590;-246.04572;,
 34.13820;41.18590;-209.70932;,
 24.13940;41.18590;-173.37320;,
 0.00000;41.18590;-158.32202;,
 -31.53950;32.09160;-162.23370;,
 0.00000;32.09160;-142.56860;,
 -44.60360;32.09160;-209.70932;,
 -31.53950;32.09160;-257.18491;,
 0.00000;32.09160;-276.84991;,
 31.53950;32.09160;-257.18491;,
 44.60360;32.09160;-209.70932;,
 31.53950;32.09160;-162.23370;,
 0.00000;32.09160;-142.56860;,
 -34.13820;21.36430;-158.32202;,
 0.00000;21.36430;-137.03680;,
 -48.27870;21.36430;-209.70932;,
 -34.13820;21.36430;-261.09641;,
 0.00000;21.36430;-282.38171;,
 34.13820;21.36430;-261.09641;,
 48.27870;21.36430;-209.70932;,
 34.13820;21.36430;-158.32202;,
 0.00000;21.36430;-137.03680;,
 -29.78270;18.98740;-164.87943;,
 0.00000;18.98740;-146.30981;,
 -27.47770;12.49190;-168.34912;,
 0.00000;12.49190;-151.21671;,
 -42.11910;18.98740;-209.71051;,
 -38.85940;12.49190;-209.71051;,
 -29.78270;18.98740;-254.54163;,
 -27.47770;12.49190;-251.07190;,
 0.00000;18.98740;-273.11130;,
 0.00000;12.49190;-268.20441;,
 29.78270;18.98740;-254.54163;,
 27.47770;12.49190;-251.07172;,
 42.11910;18.98740;-209.71051;,
 38.85940;12.49190;-209.71051;,
 29.78270;18.98740;-164.87943;,
 27.47770;12.49190;-168.34912;,
 0.00000;18.98740;-146.30981;,
 0.00000;12.49190;-151.21671;,
 -20.95760;6.99560;-178.16382;,
 0.00000;6.99560;-165.09641;,
 -29.63860;6.99560;-209.71051;,
 -20.95760;6.99560;-241.25760;,
 0.00000;6.99560;-254.32452;,
 20.95760;6.99560;-241.25760;,
 29.63860;6.99560;-209.71051;,
 20.95760;6.99560;-178.16382;,
 0.00000;6.99560;-165.09641;,
 -11.30050;1.80910;-192.70032;,
 0.00000;1.80910;-185.65430;,
 -15.98130;1.80910;-209.71051;,
 -11.30050;1.80910;-226.72070;,
 0.00000;1.80910;-233.76672;,
 11.30050;1.80910;-226.72070;,
 15.98130;1.80910;-209.71051;,
 11.30050;1.80910;-192.70032;,
 0.00000;1.80910;-185.65430;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00460;-209.71051;,
 0.00000;0.00000;-0.00430;,
 0.00000;0.00000;5.96850;,
 -4.22340;0.00000;4.21910;,
 0.00000;0.00000;-0.00430;,
 -5.97280;0.00000;-0.00430;,
 0.00000;0.00000;-0.00430;,
 -4.22340;0.00000;-4.22760;,
 0.00000;0.00000;-0.00430;,
 0.00000;0.00000;-5.97700;,
 0.00000;0.00000;-0.00430;,
 4.22340;0.00000;-4.22760;,
 0.00000;0.00000;-0.00430;,
 5.97280;0.00000;-0.00430;,
 0.00000;0.00000;-0.00430;,
 4.22340;0.00000;4.21910;,
 0.00000;0.00000;-0.00430;,
 0.00000;0.00000;5.96850;,
 0.00000;21.03440;-11.90540;,
 -3.92490;22.66090;-10.75630;,
 -3.92550;22.65910;-153.01590;,
 0.00000;21.03320;-153.01590;,
 -5.55140;26.58580;-7.98060;,
 -5.55140;26.58460;-153.01590;,
 -3.92490;30.51060;-5.20490;,
 -3.92550;30.51000;-153.01590;,
 0.00000;32.13650;-4.05530;,
 0.00000;32.13600;-153.01590;,
 3.92490;30.51060;-5.20490;,
 3.92540;30.51000;-153.01590;,
 5.55140;26.58580;-7.98060;,
 5.55140;26.58460;-153.01590;,
 3.92490;22.66090;-10.75630;,
 3.92540;22.65910;-153.01590;,
 0.00000;21.03440;-11.90540;,
 0.00000;21.03320;-153.01590;,
 4.07410;11.60680;2.83610;,
 4.22340;0.00000;4.21910;,
 5.97280;0.00000;-0.00430;,
 5.76210;9.64440;-0.66340;,
 0.00000;12.41980;4.28570;,
 0.00000;0.00000;5.96850;,
 4.22340;0.00000;-4.22760;,
 4.07410;7.68200;-4.16290;,
 -4.07410;11.60680;2.83610;,
 -4.22340;0.00000;4.21910;,
 0.00000;0.00000;-5.97700;,
 0.00000;6.86870;-5.61220;,
 -5.76210;9.64440;-0.66340;,
 -5.97280;0.00000;-0.00430;,
 0.00000;6.86870;-5.61220;,
 0.00000;0.00000;-5.97700;,
 -4.22340;0.00000;-4.22760;,
 -4.07410;7.68200;-4.16290;;
 
 104;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;44,45,36,35;,
 4;45,44,46,47;,
 4;48,44,35,37;,
 4;44,48,49,46;,
 4;50,48,37,38;,
 4;48,50,51,49;,
 4;52,50,38,39;,
 4;50,52,53,51;,
 4;54,52,39,40;,
 4;52,54,55,53;,
 4;56,54,40,41;,
 4;54,56,57,55;,
 4;58,56,41,42;,
 4;56,58,59,57;,
 4;60,58,42,43;,
 4;58,60,61,59;,
 4;47,46,62,63;,
 4;46,49,64,62;,
 4;49,51,65,64;,
 4;51,53,66,65;,
 4;53,55,67,66;,
 4;55,57,68,67;,
 4;57,59,69,68;,
 4;59,61,70,69;,
 4;63,62,71,72;,
 4;62,64,73,71;,
 4;64,65,74,73;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;69,70,79,78;,
 3;72,71,80;,
 3;71,73,81;,
 3;73,74,82;,
 3;74,75,83;,
 3;75,76,84;,
 3;76,77,85;,
 3;77,78,86;,
 3;78,79,87;,
 3;88,89,90;,
 3;91,90,92;,
 3;93,92,94;,
 3;95,94,96;,
 3;97,96,98;,
 3;99,98,100;,
 3;101,100,102;,
 3;103,102,104;,
 4;105,106,107,108;,
 4;106,109,110,107;,
 4;109,111,112,110;,
 4;111,113,114,112;,
 4;113,115,116,114;,
 4;115,117,118,116;,
 4;117,119,120,118;,
 4;119,121,122,120;,
 4;123,124,125,126;,
 4;123,126,117,115;,
 4;123,115,113,127;,
 4;123,127,128,124;,
 4;126,125,129,130;,
 4;126,130,119,117;,
 4;127,113,111,131;,
 4;127,131,132,128;,
 4;130,129,133,134;,
 4;130,134,121,119;,
 4;131,111,109,135;,
 4;131,135,136,132;,
 4;137,138,139,140;,
 4;137,140,106,105;,
 4;135,109,106,140;,
 4;135,140,139,136;;
 
 MeshMaterialList {
  3;
  104;
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
  1,
  0,
  1,
  0,
  1,
  2,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
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
   0.599200;0.599200;0.599200;1.000000;;
   20.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.313600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.600000;0.600000;0.235200;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  107;
  0.000000;1.000000;0.000000;,
  -0.000000;0.986593;0.163200;,
  -0.170791;0.978619;0.114608;,
  -0.239887;0.970801;0.000000;,
  -0.170791;0.978619;-0.114608;,
  -0.000000;0.986593;-0.163200;,
  0.170791;0.978619;-0.114607;,
  0.239887;0.970801;0.000000;,
  0.170790;0.978619;0.114607;,
  -0.000000;0.926592;0.376069;,
  -0.375454;0.890243;0.257879;,
  -0.515662;0.856792;0.000000;,
  -0.375454;0.890243;-0.257880;,
  -0.000000;0.926591;-0.376071;,
  0.375454;0.890243;-0.257880;,
  0.515662;0.856792;0.000000;,
  0.375454;0.890243;0.257878;,
  -0.000000;0.699373;0.714757;,
  -0.636705;0.620631;0.457629;,
  -0.828163;0.560487;-0.000000;,
  -0.636705;0.620629;-0.457631;,
  0.000000;0.699369;-0.714760;,
  0.636706;0.620628;-0.457631;,
  0.828164;0.560486;-0.000000;,
  0.636705;0.620631;0.457629;,
  0.000000;0.458330;0.888782;,
  -0.745190;0.380273;0.547800;,
  -0.946023;0.324100;-0.000000;,
  -0.745191;0.380269;-0.547801;,
  0.000000;0.458324;-0.888785;,
  0.745191;0.380269;-0.547801;,
  0.946023;0.324100;-0.000000;,
  0.745190;0.380273;0.547800;,
  0.000000;-0.841833;0.539738;,
  -0.502260;-0.787545;0.357081;,
  -0.668543;-0.743674;-0.000003;,
  -0.502276;-0.787528;-0.357098;,
  -0.000000;-0.841815;-0.539766;,
  0.502275;-0.787529;-0.357096;,
  0.668542;-0.743674;-0.000003;,
  0.502260;-0.787545;0.357081;,
  0.000000;-0.802965;0.596026;,
  -0.551265;-0.736787;0.391474;,
  -0.730292;-0.683135;0.000000;,
  -0.551265;-0.736787;-0.391474;,
  0.000000;-0.802966;-0.596025;,
  0.551265;-0.736788;-0.391473;,
  0.730292;-0.683136;-0.000000;,
  0.551265;-0.736787;0.391474;,
  0.000000;-0.951836;0.306608;,
  -0.313481;-0.925493;0.212584;,
  -0.434976;-0.900442;0.000000;,
  -0.313480;-0.925493;-0.212585;,
  0.000000;-0.951836;-0.306609;,
  0.313481;-0.925493;-0.212585;,
  0.434976;-0.900442;0.000000;,
  0.313481;-0.925493;0.212584;,
  0.000000;-0.987139;0.159863;,
  -0.167407;-0.979472;0.112295;,
  -0.235194;-0.971948;0.000000;,
  -0.167406;-0.979472;-0.112294;,
  0.000000;-0.987139;-0.159862;,
  0.167406;-0.979472;-0.112294;,
  0.235194;-0.971948;0.000000;,
  0.167407;-0.979472;0.112295;,
  -0.000000;-1.000000;0.000000;,
  0.000000;-0.968684;0.248297;,
  -0.257702;-0.950541;0.173380;,
  -0.360012;-0.932948;-0.000005;,
  -0.257740;-0.950524;-0.173417;,
  0.000000;-0.968667;-0.248362;,
  0.257740;-0.950524;-0.173417;,
  0.360012;-0.932948;-0.000005;,
  0.257702;-0.950541;0.173380;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.033969;-0.999423;,
  -0.688140;0.020289;-0.725294;,
  -0.999460;0.032185;-0.006694;,
  -0.695406;0.093689;0.712484;,
  0.000000;0.128929;0.991654;,
  0.695406;0.093689;0.712484;,
  0.999460;0.032185;-0.006694;,
  0.688140;0.020289;-0.725294;,
  0.000003;-1.000000;0.000011;,
  -0.768396;-0.534741;-0.351596;,
  -0.999990;0.003408;-0.002863;,
  -0.768581;0.535592;0.349891;,
  0.000002;1.000000;-0.000003;,
  0.768580;0.535594;0.349890;,
  0.999990;0.003408;-0.002864;,
  0.768395;-0.534742;-0.351596;,
  -0.707105;-0.707108;0.000009;,
  -1.000000;0.000004;0.000003;,
  -0.707084;0.707130;-0.000002;,
  0.707082;0.707132;-0.000002;,
  1.000000;0.000004;0.000003;,
  0.707103;-0.707110;0.000009;,
  0.706409;0.188028;0.682372;,
  0.999792;0.019436;-0.006208;,
  0.000000;0.262321;0.964981;,
  0.707133;-0.130819;-0.694873;,
  -0.706409;0.188028;0.682372;,
  0.000000;-0.187635;-0.982239;,
  -0.999792;0.019436;-0.006208;,
  -0.707133;-0.130819;-0.694873;,
  0.000000;0.391885;0.920014;,
  0.000000;-0.402643;-0.915357;;
  104;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
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
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;34,33,66,67;,
  4;33,34,42,41;,
  4;35,34,67,68;,
  4;34,35,43,42;,
  4;36,35,68,69;,
  4;35,36,44,43;,
  4;37,36,69,70;,
  4;36,37,45,44;,
  4;38,37,70,71;,
  4;37,38,46,45;,
  4;39,38,71,72;,
  4;38,39,47,46;,
  4;40,39,72,73;,
  4;39,40,48,47;,
  4;33,40,73,66;,
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
  3;57,58,65;,
  3;58,59,65;,
  3;59,60,65;,
  3;60,61,65;,
  3;61,62,65;,
  3;62,63,65;,
  3;63,64,65;,
  3;64,57,65;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  3;74,74,74;,
  4;83,84,91,83;,
  4;84,85,92,91;,
  4;85,86,93,92;,
  4;86,87,87,93;,
  4;87,88,94,87;,
  4;88,89,95,94;,
  4;89,90,96,95;,
  4;90,83,83,96;,
  4;97,80,81,98;,
  4;97,98,89,88;,
  4;97,88,105,99;,
  4;97,99,79,80;,
  4;98,81,82,100;,
  4;98,100,90,89;,
  4;99,105,86,101;,
  4;99,101,78,79;,
  4;100,82,75,102;,
  4;100,102,106,90;,
  4;101,86,85,103;,
  4;101,103,77,78;,
  4;102,75,76,104;,
  4;102,104,84,106;,
  4;103,85,84,104;,
  4;103,104,76,77;;
 }
 MeshTextureCoords {
  141;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.500000;,
  0.250000;0.625000;,
  0.375000;0.500000;,
  0.375000;0.625000;,
  0.500000;0.500000;,
  0.500000;0.625000;,
  0.625000;0.500000;,
  0.625000;0.625000;,
  0.750000;0.500000;,
  0.750000;0.625000;,
  0.875000;0.500000;,
  0.875000;0.625000;,
  1.000000;0.500000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
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
  0.375000;0.000000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.500000;0.000000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.312500;0.625000;,
  0.250000;0.625000;,
  0.250000;0.750000;,
  0.312500;0.750000;,
  0.312500;0.500000;,
  0.250000;0.500000;,
  0.250000;0.875000;,
  0.312500;0.875000;,
  0.312500;0.375000;,
  0.250000;0.375000;,
  0.250000;1.000000;,
  0.312500;1.000000;,
  0.312500;0.250000;,
  0.250000;0.250000;,
  0.312500;0.000000;,
  0.250000;0.000000;,
  0.250000;0.125000;,
  0.312500;0.125000;;
 }
}
