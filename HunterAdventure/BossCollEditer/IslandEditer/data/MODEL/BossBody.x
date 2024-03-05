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
 137;
 -158.44110;227.98595;-669.00183;,
 -0.00120;227.98595;-687.12964;,
 -0.00120;-139.04099;-687.12964;,
 -158.44110;19.12835;-669.00183;,
 158.43900;227.98595;-669.00183;,
 158.43900;19.12835;-669.00183;,
 -216.93480;295.73607;-641.74649;,
 -158.44110;227.98595;-669.00183;,
 -158.44110;19.12835;-669.00183;,
 -216.93480;9.77129;-641.74649;,
 -216.93480;9.77129;-601.40520;,
 -216.93480;295.73607;-601.40520;,
 -216.93480;295.73607;-641.74649;,
 -216.93480;295.73607;-601.40520;,
 -0.00120;334.72618;-601.40552;,
 -0.00120;334.72618;-641.74649;,
 -0.00120;227.98595;-687.12964;,
 -216.93480;9.77129;-641.74649;,
 -0.00120;-139.04099;-687.12964;,
 -0.00120;-206.79149;-641.74649;,
 -0.00120;-206.79149;-601.40552;,
 -216.93480;9.77129;-601.40520;,
 216.93240;295.73607;-601.40552;,
 216.93240;295.73607;-641.74649;,
 158.43900;227.98595;-669.00183;,
 158.43900;19.12835;-669.00183;,
 216.93240;9.77129;-641.74649;,
 216.93240;9.77129;-601.40552;,
 216.93240;295.73607;-641.74649;,
 216.93240;295.73607;-601.40552;,
 216.93240;9.77129;-601.40552;,
 216.93240;9.77129;-641.74649;,
 158.43900;19.12835;-669.00183;,
 158.43900;227.98595;-669.00183;,
 244.51769;325.20781;-498.76199;,
 -0.00120;369.15569;-498.76199;,
 -0.00120;-241.22099;-498.76199;,
 244.51769;2.87975;-498.76199;,
 -244.51980;325.20781;-498.76199;,
 -244.51980;2.87975;-498.76199;,
 -244.51980;325.20781;-601.39531;,
 -244.51980;2.87975;-601.39531;,
 -244.51980;2.87975;-498.76199;,
 -244.51980;325.20781;-498.76199;,
 -244.51980;325.20781;-601.39531;,
 -244.51980;325.20781;-498.76199;,
 -0.00120;369.15569;-498.76199;,
 -0.00120;369.15569;-601.39531;,
 -244.51980;2.87975;-601.39531;,
 -0.00120;-241.22099;-601.39531;,
 -0.00120;-241.22099;-498.76199;,
 -244.51980;2.87975;-498.76199;,
 244.51769;325.20781;-498.76199;,
 244.51769;325.20781;-601.39531;,
 244.51769;2.87975;-601.39531;,
 244.51769;2.87975;-498.76199;,
 244.51769;325.20781;-601.39531;,
 244.51769;325.20781;-498.76199;,
 244.51769;2.87975;-498.76199;,
 244.51769;2.87975;-601.39531;,
 -0.00120;369.15569;-601.39531;,
 -0.00120;334.72618;-601.40552;,
 -216.93480;295.73607;-601.40520;,
 -244.51980;325.20781;-601.39531;,
 -0.00120;334.72618;-601.40552;,
 244.51769;325.20781;-601.39531;,
 216.93240;295.73607;-601.40552;,
 244.51769;2.87975;-601.39531;,
 216.93240;9.77129;-601.40552;,
 216.93240;295.73607;-601.40552;,
 216.93240;9.77129;-601.40552;,
 -0.00120;-241.22099;-601.39531;,
 -0.00120;-206.79149;-601.40552;,
 -0.00120;-206.79149;-601.40552;,
 -244.51980;2.87975;-601.39531;,
 -216.93480;9.77129;-601.40520;,
 -216.93480;9.77129;-601.40520;,
 -216.93480;295.73607;-601.40520;,
 0.00000;345.62489;-0.00211;,
 161.24010;278.83708;-0.00211;,
 161.24010;278.83379;-498.75000;,
 0.00000;345.62160;-498.75000;,
 161.24010;278.83708;-0.00211;,
 228.02790;117.59735;-0.00211;,
 228.02790;117.59405;-498.75000;,
 161.24010;278.83379;-498.75000;,
 161.24010;-174.45057;-0.00211;,
 161.24010;-174.45423;-498.75000;,
 161.24010;-174.45057;-0.00211;,
 0.00000;-241.23839;-0.00211;,
 0.00000;-241.24200;-498.75000;,
 161.24010;-174.45423;-498.75000;,
 -161.24010;-174.45057;-0.00211;,
 -161.24010;-174.45423;-498.75000;,
 -161.24010;-174.45057;-0.00211;,
 -228.02790;117.59735;-0.00211;,
 -228.02790;117.59405;-498.75000;,
 -161.24010;-174.45423;-498.75000;,
 -161.24010;278.83708;-0.00211;,
 -161.24010;278.83379;-498.75000;,
 -161.24010;278.83708;-0.00211;,
 -161.24010;278.83379;-498.75000;,
 0.00000;117.59735;-0.00211;,
 161.24010;278.83708;-0.00211;,
 0.00000;345.62489;-0.00211;,
 228.02790;117.59735;-0.00211;,
 161.24010;-174.45057;-0.00211;,
 0.00000;-241.23839;-0.00211;,
 -161.24010;-174.45057;-0.00211;,
 -161.24010;278.83708;-0.00211;,
 0.00000;117.59405;-498.75000;,
 0.00000;345.62160;-498.75000;,
 161.24010;278.83379;-498.75000;,
 228.02790;117.59405;-498.75000;,
 161.24010;-174.45423;-498.75000;,
 0.00000;-241.24200;-498.75000;,
 -161.24010;-174.45423;-498.75000;,
 -228.02790;117.59405;-498.75000;,
 -161.24010;278.83379;-498.75000;,
 -17.65230;427.77269;-319.37160;,
 17.65230;427.77269;-319.37160;,
 17.65230;318.53819;-319.37160;,
 -17.65230;318.53819;-319.37160;,
 17.65230;427.77269;-319.37160;,
 17.65230;427.77269;-25.33321;,
 17.65230;318.53819;-25.33321;,
 17.65230;318.53819;-319.37160;,
 17.65230;427.77269;-25.33321;,
 -17.65230;427.77269;-25.33321;,
 -17.65230;318.53819;-25.33321;,
 17.65230;318.53819;-25.33321;,
 -17.65230;427.77269;-25.33321;,
 -17.65230;427.77269;-319.37160;,
 -17.65230;318.53819;-319.37160;,
 -17.65230;318.53819;-25.33321;,
 17.65230;427.77269;-319.37160;,
 -17.65230;427.77269;-319.37160;;
 
 57;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;6,9,10,11;,
 4;12,13,14,15;,
 4;12,15,16,0;,
 4;17,3,18,19;,
 4;17,19,20,21;,
 4;15,14,22,23;,
 4;15,23,24,16;,
 4;19,18,25,26;,
 4;19,26,27,20;,
 4;28,29,30,31;,
 4;28,31,32,33;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;47,46,52,53;,
 4;49,54,55,50;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,60,65,66;,
 4;67,68,69,65;,
 4;70,67,71,72;,
 4;73,71,74,75;,
 4;76,74,63,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;83,86,87,84;,
 4;88,89,90,91;,
 4;89,92,93,90;,
 4;94,95,96,97;,
 4;95,98,99,96;,
 4;100,78,81,101;,
 3;102,103,104;,
 3;102,105,103;,
 3;102,106,105;,
 3;102,107,106;,
 3;102,108,107;,
 3;102,95,108;,
 3;102,109,95;,
 3;102,104,109;,
 3;110,111,112;,
 3;110,112,113;,
 3;110,113,114;,
 3;110,114,115;,
 3;110,115,116;,
 3;110,116,117;,
 3;110,117,118;,
 3;110,118,111;,
 4;119,120,121,122;,
 4;123,124,125,126;,
 4;127,128,129,130;,
 4;131,132,133,134;,
 4;131,124,135,136;;
 
 MeshMaterialList {
  6;
  57;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  3,
  3,
  1,
  2,
  2,
  2,
  2,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  2,
  1,
  1,
  2,
  2,
  1,
  1,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.627200;0.000000;0.000000;1.000000;;
   5.000000;
   1.000000;1.000000;1.000000;;
   0.784000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\BossSkin.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\BossSkin.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\BossSkin.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\GoldGloss.png";
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
  57;
  0.211735;0.123010;-0.969555;,
  0.341126;-0.142385;-0.929171;,
  0.176898;0.984229;0.000000;,
  0.000000;1.000000;0.000000;,
  0.923716;-0.383079;-0.000000;,
  -0.706502;-0.707711;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.923716;-0.383079;0.000000;,
  -0.211735;0.123010;-0.969555;,
  -0.000000;0.183499;-0.983020;,
  -0.341126;-0.142385;-0.929172;,
  -0.000000;-0.225140;-0.974326;,
  -0.260248;0.185276;-0.947599;,
  -0.447561;-0.211099;-0.868980;,
  -0.645336;-0.623977;-0.440675;,
  0.785437;-0.340886;-0.516610;,
  -0.176898;0.984229;0.000000;,
  -0.000000;0.360220;-0.932867;,
  0.260249;0.185276;-0.947599;,
  0.645336;-0.623978;-0.440674;,
  0.706502;-0.707711;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  -0.923716;-0.383078;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.706502;-0.707711;0.000000;,
  0.176898;0.984229;0.000000;,
  0.923716;-0.383079;-0.000000;,
  -0.000053;0.000292;-1.000000;,
  0.000053;0.000296;-1.000000;,
  0.000370;0.000000;-1.000000;,
  0.000296;-0.000296;-1.000000;,
  -0.000292;-0.000292;-1.000000;,
  -0.000358;0.000000;-1.000000;,
  -0.176898;0.984229;0.000000;,
  0.706501;-0.707712;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000294;-1.000000;,
  -0.000206;0.000146;-1.000000;,
  0.000212;0.000148;-1.000000;,
  0.000333;-0.000148;-1.000000;,
  0.000002;-0.000294;-1.000000;,
  -0.000325;-0.000146;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;-0.000007;,
  0.707107;0.707107;-0.000005;,
  0.996479;0.083841;-0.000001;,
  0.763901;-0.645333;0.000005;,
  0.000000;-1.000000;0.000007;,
  -0.763901;-0.645333;0.000005;,
  -0.996479;0.083841;-0.000001;,
  -0.707107;0.707107;-0.000005;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;;
  57;
  4;8,9,11,10;,
  4;9,0,1,11;,
  4;12,8,10,13;,
  4;6,7,7,6;,
  4;16,16,3,3;,
  4;12,17,9,8;,
  4;13,10,11,14;,
  4;7,14,5,7;,
  4;3,3,2,2;,
  4;17,18,0,9;,
  4;19,11,1,15;,
  4;19,15,4,20;,
  4;21,21,4,15;,
  4;18,15,1,0;,
  4;22,22,22,22;,
  4;22,22,22,22;,
  4;23,24,24,23;,
  4;35,35,25,25;,
  4;24,26,26,24;,
  4;25,25,27,27;,
  4;36,28,28,36;,
  4;37,37,28,28;,
  4;38,29,29,39;,
  4;30,38,40,30;,
  4;41,31,31,40;,
  4;32,41,42,32;,
  4;33,42,43,33;,
  4;34,43,39,34;,
  4;45,46,46,45;,
  4;46,47,47,46;,
  4;47,48,48,47;,
  4;48,49,49,48;,
  4;49,50,50,49;,
  4;50,51,51,50;,
  4;51,52,52,51;,
  4;52,45,45,52;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;44,44,44;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  3;53,53,53;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;44,44,44,44;,
  4;55,55,55,55;,
  4;56,56,56,56;;
 }
 MeshTextureCoords {
  137;
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.937180;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.937180;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.062820;0.000000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.062820;0.500000;,
  0.000000;0.500000;,
  0.062820;1.000000;,
  0.000000;0.500000;,
  0.062820;0.500000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.062820;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.062820;0.000000;,
  1.000000;0.000000;,
  0.062820;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062820;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.907530;-2.642810;,
  0.500000;-2.716060;,
  0.500000;-1.698760;,
  0.907530;-2.105600;,
  0.092470;-2.642810;,
  0.092470;-2.105600;,
  1.122280;-2.642810;,
  1.122280;-2.105600;,
  0.951220;-2.105600;,
  0.951220;-2.642810;,
  0.092470;1.122280;,
  0.092470;0.951220;,
  0.500000;0.951220;,
  0.500000;1.122280;,
  0.092470;1.122280;,
  0.500000;1.122280;,
  0.500000;0.951220;,
  0.092470;0.951220;,
  0.907530;0.951220;,
  0.907530;1.122280;,
  0.907530;1.122280;,
  0.907530;0.951220;,
  1.122280;-2.642810;,
  0.951220;-2.642810;,
  0.951220;-2.105600;,
  1.122280;-2.105600;,
  0.500000;-2.716060;,
  0.500000;-2.658680;,
  0.138440;-2.593690;,
  0.092470;-2.642810;,
  0.500000;-2.658680;,
  0.907530;-2.642810;,
  0.861550;-2.593690;,
  0.907530;-2.105600;,
  0.861550;-2.117090;,
  0.861550;-2.593690;,
  0.861550;-2.117090;,
  0.500000;-1.698760;,
  0.500000;-1.756150;,
  0.500000;-1.756150;,
  0.092470;-2.105600;,
  0.138440;-2.117090;,
  0.138440;-2.117090;,
  0.138440;-2.593690;,
  0.500000;0.119950;,
  0.768730;0.119950;,
  0.768730;0.951200;,
  0.500000;0.951200;,
  0.119950;-2.565530;,
  0.119950;-2.296800;,
  0.951200;-2.296790;,
  0.951200;-2.565520;,
  0.119950;-1.810050;,
  0.951200;-1.810040;,
  0.768730;0.119950;,
  0.500000;0.119950;,
  0.500000;0.951200;,
  0.768730;0.951200;,
  0.231270;0.119950;,
  0.231270;0.951200;,
  0.119950;-1.810050;,
  0.119950;-2.296800;,
  0.951200;-2.296790;,
  0.951200;-1.810040;,
  0.119950;-2.565530;,
  0.951200;-2.565520;,
  0.231270;0.119950;,
  0.231270;0.951200;,
  0.500000;-2.296800;,
  0.768730;-2.565530;,
  0.500000;-2.676840;,
  0.880050;-2.296800;,
  0.768730;-1.810050;,
  0.500000;-1.698740;,
  0.231270;-1.810050;,
  0.231270;-2.565530;,
  0.500000;-2.296790;,
  0.500000;-2.676840;,
  0.768730;-2.565520;,
  0.880050;-2.296790;,
  0.768730;-1.810040;,
  0.500000;-1.698730;,
  0.231270;-1.810040;,
  0.119950;-2.296790;,
  0.231270;-2.565520;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;;
 }
}
