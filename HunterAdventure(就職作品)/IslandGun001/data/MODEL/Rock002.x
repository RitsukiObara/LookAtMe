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
 144;
 -198.46111;50.93790;92.88880;,
 -203.69640;51.87590;17.66910;,
 -190.18303;0.00000;6.79250;,
 -177.14905;0.00000;70.47190;,
 -141.02567;0.00000;132.03873;,
 -164.40240;50.00000;168.10860;,
 -135.65900;169.38995;141.61880;,
 -185.47220;171.70905;76.26880;,
 -189.68820;175.92015;10.91890;,
 -136.28116;315.06616;129.85081;,
 -159.05731;318.00897;71.18121;,
 -190.14470;309.51871;12.51162;,
 176.85062;0.00000;70.47190;,
 185.06969;0.00000;6.79250;,
 205.53513;51.87590;17.66910;,
 179.11270;50.93790;92.88880;,
 126.86140;50.00000;168.10860;,
 141.18944;0.00000;140.49416;,
 -128.72775;393.56808;115.73000;,
 -131.55016;453.70062;41.46456;,
 -147.07278;452.61493;-5.49265;,
 196.36203;174.02826;10.91890;,
 155.46320;171.70905;76.26880;,
 126.86140;169.38995;141.61880;,
 166.47108;320.95172;12.51162;,
 151.23318;318.00897;71.18121;,
 125.19693;315.06616;129.85081;,
 97.81898;464.43832;41.46456;,
 82.73762;406.34888;106.78863;,
 104.15236;474.63837;-5.49265;,
 -198.46111;52.81380;-57.55070;,
 -164.40240;53.75180;-132.77049;,
 -163.80576;0.00000;-106.54530;,
 -187.24872;0.00000;-56.88690;,
 -185.47220;176.34735;-54.43100;,
 -135.65900;178.66656;-119.78100;,
 -159.05731;323.89450;-46.15796;,
 -136.28116;326.83734;-104.82758;,
 172.68188;0.00000;-56.88690;,
 133.64424;0.00000;-107.90541;,
 132.91740;53.75180;-125.31010;,
 193.61493;52.81380;-57.55070;,
 -114.96123;448.78574;-37.21620;,
 -114.96123;427.12796;-76.55652;,
 126.86140;178.66656;-112.41690;,
 169.96542;176.34735;-54.43100;,
 112.30225;341.44382;-97.56169;,
 151.23318;323.89450;-46.15796;,
 97.81898;472.09576;-37.21620;,
 94.08067;442.95120;-79.70531;,
 23.83311;433.06332;152.82764;,
 -22.43170;435.84235;159.70287;,
 -5.54211;301.83792;178.30956;,
 73.51690;315.06616;167.20514;,
 125.19693;315.06616;129.85081;,
 82.73762;406.34888;106.78863;,
 23.83311;433.06332;152.82764;,
 97.81898;464.43832;41.46456;,
 44.62390;536.20428;41.46456;,
 -8.57119;539.64447;80.28587;,
 -4.39880;161.28436;190.25780;,
 68.55230;169.38995;191.63266;,
 126.86140;169.38995;141.61880;,
 104.15236;474.63837;-5.49265;,
 54.19910;534.09802;-5.49265;,
 -12.89157;549.50836;-5.49265;,
 -18.77050;59.42228;209.00079;,
 54.04540;50.00000;194.65430;,
 126.86140;50.00000;168.10860;,
 97.81898;472.09576;-37.21620;,
 44.62390;524.52631;-62.28279;,
 -8.57119;513.95673;-81.31847;,
 -15.67260;0.00000;195.35580;,
 63.57450;0.00000;174.16983;,
 141.18944;0.00000;140.49416;,
 94.08067;442.95120;-79.70531;,
 34.80185;444.61060;-120.51208;,
 -22.43170;419.52725;-133.45311;,
 63.57450;0.00000;174.16983;,
 12.02190;-79.65219;106.93550;,
 87.04430;-64.73659;70.47190;,
 176.85062;0.00000;70.47190;,
 34.80185;444.61060;-120.51208;,
 94.08067;442.95120;-79.70531;,
 112.30225;341.44382;-97.56169;,
 59.82735;326.83734;-122.80157;,
 -5.54211;312.53140;-138.88036;,
 -22.43170;419.52725;-133.45311;,
 -5.13930;-90.84659;6.79250;,
 117.43350;-79.57349;6.79250;,
 185.06969;0.00000;6.79250;,
 126.86140;178.66656;-112.41690;,
 61.23130;178.66656;-142.59470;,
 -4.39880;162.85986;-153.46201;,
 -7.36800;-58.23480;-95.21720;,
 87.04430;-57.20940;-56.88690;,
 172.68188;0.00000;-56.88690;,
 132.91740;53.75180;-125.31010;,
 54.04540;53.75180;-151.47200;,
 -18.77050;35.71840;-161.34500;,
 -15.67260;0.00000;-149.31593;,
 63.57450;0.00000;-130.97752;,
 133.64424;0.00000;-107.90541;,
 133.64424;0.00000;-107.90541;,
 63.57450;0.00000;-130.97752;,
 -15.67260;0.00000;-149.31593;,
 -68.69643;435.73865;137.58824;,
 -128.72775;393.56808;115.73000;,
 -136.28116;315.06616;129.85081;,
 -70.91168;315.06616;157.88005;,
 -68.69643;435.73865;137.58824;,
 -81.63032;517.78680;59.14321;,
 -131.55016;453.70062;41.46456;,
 -135.65900;169.38995;141.61880;,
 -70.02890;169.38995;178.65669;,
 -79.98225;519.52191;-5.49265;,
 -147.07278;452.61493;-5.49265;,
 -164.40240;50.00000;168.10860;,
 -91.58650;50.00000;194.65430;,
 -61.76628;512.87152;-62.28279;,
 -114.96123;448.78574;-37.21620;,
 -141.02567;0.00000;132.03873;,
 -86.07967;0.00000;173.99869;,
 -83.94528;432.95554;-99.27214;,
 -114.96123;427.12796;-76.55652;,
 -86.07967;0.00000;173.99869;,
 -177.14905;0.00000;70.47190;,
 -101.78020;-60.03820;70.47190;,
 -83.94528;432.95554;-99.27214;,
 -70.91168;326.83734;-122.80157;,
 -136.28116;326.83734;-104.82758;,
 -114.96123;427.12796;-76.55652;,
 -190.18303;0.00000;6.79250;,
 -104.63110;-60.03820;6.79250;,
 -70.02890;178.66656;-142.59470;,
 -135.65900;178.66656;-119.78100;,
 -187.24872;0.00000;-56.88690;,
 -101.78020;-47.08870;-56.88690;,
 -91.58650;53.75180;-151.47200;,
 -164.40240;53.75180;-132.77049;,
 -163.80576;0.00000;-106.54530;,
 -124.98154;0.00000;-126.79031;,
 -124.98154;0.00000;-126.79031;,
 -163.80576;0.00000;-106.54530;;
 
 96;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;0,5,6,7;,
 4;0,7,8,1;,
 4;7,6,9,10;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;12,15,16,17;,
 4;10,9,18,19;,
 4;10,19,20,11;,
 4;15,14,21,22;,
 4;15,22,23,16;,
 4;22,21,24,25;,
 4;22,25,26,23;,
 4;27,28,26,25;,
 4;27,25,24,29;,
 4;30,31,32,33;,
 4;30,33,2,1;,
 4;30,1,8,34;,
 4;30,34,35,31;,
 4;34,8,11,36;,
 4;34,36,37,35;,
 4;38,39,40,41;,
 4;38,41,14,13;,
 4;36,11,20,42;,
 4;36,42,43,37;,
 4;41,40,44,45;,
 4;41,45,21,14;,
 4;45,44,46,47;,
 4;45,47,24,21;,
 4;48,29,24,47;,
 4;48,47,46,49;,
 4;50,51,52,53;,
 4;50,53,54,55;,
 4;56,28,57,58;,
 4;56,58,59,51;,
 4;53,52,60,61;,
 4;53,61,62,54;,
 4;58,57,63,64;,
 4;58,64,65,59;,
 4;61,60,66,67;,
 4;61,67,68,62;,
 4;64,63,69,70;,
 4;64,70,71,65;,
 4;67,66,72,73;,
 4;67,73,74,68;,
 4;70,69,75,76;,
 4;70,76,77,71;,
 4;78,72,79,80;,
 4;78,80,81,17;,
 4;82,83,84,85;,
 4;82,85,86,87;,
 4;80,79,88,89;,
 4;80,89,90,81;,
 4;85,84,91,92;,
 4;85,92,93,86;,
 4;89,88,94,95;,
 4;89,95,96,90;,
 4;92,91,97,98;,
 4;92,98,99,93;,
 4;95,94,100,101;,
 4;95,101,102,96;,
 4;98,97,103,104;,
 4;98,104,105,99;,
 4;106,107,108,109;,
 4;106,109,52,51;,
 4;110,51,59,111;,
 4;110,111,112,18;,
 4;109,108,113,114;,
 4;109,114,60,52;,
 4;111,59,65,115;,
 4;111,115,116,112;,
 4;114,113,117,118;,
 4;114,118,66,60;,
 4;115,65,71,119;,
 4;115,119,120,116;,
 4;118,117,121,122;,
 4;118,122,72,66;,
 4;119,71,77,123;,
 4;119,123,124,120;,
 4;125,4,126,127;,
 4;125,127,79,72;,
 4;128,87,86,129;,
 4;128,129,130,131;,
 4;127,126,132,133;,
 4;127,133,88,79;,
 4;129,86,93,134;,
 4;129,134,135,130;,
 4;133,132,136,137;,
 4;133,137,94,88;,
 4;134,93,99,138;,
 4;134,138,139,135;,
 4;137,136,140,141;,
 4;137,141,100,94;,
 4;138,99,105,142;,
 4;138,142,143,139;;
 
 MeshMaterialList {
  1;
  96;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\Rock003.png";
   }
  }
 }
 MeshNormals {
  98;
  -0.727147;0.369543;-0.578529;,
  -0.591868;-0.522446;-0.613794;,
  0.683246;0.336904;-0.647819;,
  0.501356;-0.466783;-0.728530;,
  0.688504;0.438987;0.577281;,
  0.584564;-0.472539;0.659539;,
  -0.719898;0.317261;0.617327;,
  -0.560442;-0.631080;0.536323;,
  0.607415;0.119349;-0.785368;,
  0.748164;0.099444;0.656019;,
  -0.617593;0.141705;-0.773627;,
  -0.665756;0.179195;0.724333;,
  0.575407;-0.088786;-0.813034;,
  0.682060;-0.063549;0.728529;,
  -0.650985;-0.063477;-0.756432;,
  -0.676850;-0.171214;0.715933;,
  0.666436;0.186714;-0.721803;,
  0.757061;0.247815;0.604522;,
  -0.685233;0.201430;-0.699915;,
  -0.707961;0.156693;0.688650;,
  -0.994739;0.102379;0.003537;,
  -0.995854;-0.088989;0.018836;,
  -0.979453;0.193905;-0.055427;,
  -0.807146;-0.590280;0.009212;,
  -0.860071;0.483612;-0.162471;,
  0.838727;-0.544552;0.000102;,
  0.860518;0.509402;-0.004261;,
  0.993615;-0.094556;0.061552;,
  0.963736;0.265312;0.028677;,
  0.985928;0.151296;0.071104;,
  -0.944841;-0.146671;0.292853;,
  -0.721469;-0.640184;0.263906;,
  -0.930720;0.140312;0.337746;,
  0.761887;-0.523297;0.381692;,
  -0.919595;0.171322;0.353545;,
  0.905544;-0.027303;0.423373;,
  -0.839646;0.427399;0.335149;,
  0.893748;0.124783;0.430864;,
  0.794056;0.484476;0.367093;,
  0.892537;0.246976;0.377333;,
  -0.962363;-0.037057;-0.269227;,
  -0.764460;-0.590334;-0.259049;,
  -0.935837;0.121633;-0.330780;,
  0.711079;-0.552464;-0.434915;,
  -0.897246;0.193598;-0.396824;,
  0.893468;-0.092269;-0.439547;,
  -0.784734;0.416392;-0.459142;,
  0.882256;0.139095;-0.449752;,
  0.808597;0.456643;-0.371010;,
  0.891775;0.235352;-0.386454;,
  -0.068849;0.405782;0.911373;,
  -0.101434;0.132568;0.985970;,
  -0.060677;0.892014;0.447917;,
  -0.065799;0.127139;0.989700;,
  -0.097260;0.985640;-0.138035;,
  -0.024811;-0.081140;0.996394;,
  -0.116601;0.771708;-0.625196;,
  0.011577;-0.563678;0.825914;,
  -0.101715;0.306770;-0.946333;,
  -0.011433;-0.922235;0.386460;,
  -0.036848;0.096306;-0.994669;,
  -0.064250;-0.994081;-0.087609;,
  0.001719;0.092399;-0.995721;,
  -0.039464;-0.871102;-0.489513;,
  0.003980;-0.125597;-0.992073;,
  0.001792;-0.552714;-0.833369;,
  0.453922;0.429542;0.780672;,
  0.366907;0.202738;0.907897;,
  0.534415;0.731572;0.423324;,
  0.325458;0.097458;0.940521;,
  0.491463;0.867225;-0.079906;,
  0.297019;-0.114111;0.948029;,
  0.428970;0.722047;-0.542801;,
  0.332887;-0.562663;0.756701;,
  0.371589;0.339186;-0.864219;,
  0.396569;-0.832593;0.386681;,
  0.322381;0.121623;-0.938764;,
  0.425449;-0.902908;-0.061234;,
  0.273392;0.093748;-0.957323;,
  0.345501;-0.802683;-0.486136;,
  0.257613;-0.117846;-0.959035;,
  0.285130;-0.535487;-0.794956;,
  -0.470352;0.370979;0.800714;,
  -0.357342;0.128426;0.925102;,
  -0.575144;0.724210;0.380432;,
  -0.314060;0.158819;0.936025;,
  -0.556274;0.813311;-0.170541;,
  -0.319578;-0.112200;0.940894;,
  -0.523767;0.643760;-0.557890;,
  -0.351841;-0.623674;0.698025;,
  -0.472561;0.335745;-0.814839;,
  -0.413491;-0.859973;0.299118;,
  -0.316259;0.128085;-0.939987;,
  -0.419365;-0.906337;-0.051821;,
  -0.221836;0.111632;-0.968673;,
  -0.351890;-0.873699;-0.335892;,
  -0.243902;-0.112522;-0.963250;,
  -0.298025;-0.609477;-0.734656;;
  96;
  4;30,21,23,31;,
  4;30,31,7,15;,
  4;30,15,11,32;,
  4;30,32,20,21;,
  4;32,11,19,34;,
  4;32,34,22,20;,
  4;33,25,27,35;,
  4;33,35,13,5;,
  4;34,19,6,36;,
  4;34,36,24,22;,
  4;35,27,29,37;,
  4;35,37,9,13;,
  4;37,29,28,39;,
  4;37,39,17,9;,
  4;38,4,17,39;,
  4;38,39,28,26;,
  4;40,14,1,41;,
  4;40,41,23,21;,
  4;40,21,20,42;,
  4;40,42,10,14;,
  4;42,20,22,44;,
  4;42,44,18,10;,
  4;43,3,12,45;,
  4;43,45,27,25;,
  4;44,22,24,46;,
  4;44,46,0,18;,
  4;45,12,8,47;,
  4;45,47,29,27;,
  4;47,8,16,49;,
  4;47,49,28,29;,
  4;48,26,28,49;,
  4;48,49,16,2;,
  4;66,50,51,67;,
  4;66,67,17,4;,
  4;66,4,38,68;,
  4;66,68,52,50;,
  4;67,51,53,69;,
  4;67,69,9,17;,
  4;68,38,26,70;,
  4;68,70,54,52;,
  4;69,53,55,71;,
  4;69,71,13,9;,
  4;70,26,48,72;,
  4;70,72,56,54;,
  4;71,55,57,73;,
  4;71,73,5,13;,
  4;72,48,2,74;,
  4;72,74,58,56;,
  4;73,57,59,75;,
  4;73,75,33,5;,
  4;74,2,16,76;,
  4;74,76,60,58;,
  4;75,59,61,77;,
  4;75,77,25,33;,
  4;76,16,8,78;,
  4;76,78,62,60;,
  4;77,61,63,79;,
  4;77,79,43,25;,
  4;78,8,12,80;,
  4;78,80,64,62;,
  4;79,63,65,81;,
  4;79,81,3,43;,
  4;80,12,3,81;,
  4;80,81,65,64;,
  4;82,6,19,83;,
  4;82,83,51,50;,
  4;82,50,52,84;,
  4;82,84,36,6;,
  4;83,19,11,85;,
  4;83,85,53,51;,
  4;84,52,54,86;,
  4;84,86,24,36;,
  4;85,11,15,87;,
  4;85,87,55,53;,
  4;86,54,56,88;,
  4;86,88,46,24;,
  4;87,15,7,89;,
  4;87,89,57,55;,
  4;88,56,58,90;,
  4;88,90,0,46;,
  4;89,7,31,91;,
  4;89,91,59,57;,
  4;90,58,60,92;,
  4;90,92,18,0;,
  4;91,31,23,93;,
  4;91,93,61,59;,
  4;92,60,62,94;,
  4;92,94,10,18;,
  4;93,23,41,95;,
  4;93,95,63,61;,
  4;94,62,64,96;,
  4;94,96,14,10;,
  4;95,41,1,97;,
  4;95,97,65,63;,
  4;96,64,65,97;,
  4;96,97,1,14;;
 }
 MeshTextureCoords {
  144;
  0.250000;0.750000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.750000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.500000;0.250000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.250000;0.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.250000;,
  0.750000;0.250000;,
  0.500000;0.250000;,
  0.500000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.750000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.750000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.750000;0.750000;,
  0.500000;0.750000;,
  0.500000;1.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.750000;0.250000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  1.000000;0.500000;,
  0.750000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  1.000000;0.750000;,
  0.750000;0.750000;,
  0.500000;0.750000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.500000;1.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.750000;0.250000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.750000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.750000;0.750000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  1.000000;1.000000;,
  0.750000;1.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;;
 }
}
