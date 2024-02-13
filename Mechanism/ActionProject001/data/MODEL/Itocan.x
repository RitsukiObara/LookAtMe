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
 252;
 -24.25297;113.19969;36.87737;,
 -25.12857;128.09378;-11.81900;,
 -27.56806;127.25611;-12.03134;,
 -26.69247;112.36197;36.66506;,
 -29.64314;124.87648;-12.72188;,
 -28.76754;109.98235;35.97452;,
 -31.03801;121.31708;-13.78549;,
 -30.16241;106.42287;34.91089;,
 -30.14048;117.12143;-15.08487;,
 -29.26489;102.22722;33.61151;,
 -25.17397;127.97139;-14.40373;,
 -28.37955;125.80889;-15.78863;,
 -25.21953;126.19825;-17.10538;,
 -30.30070;122.17801;-16.61126;,
 -28.18521;122.81008;-18.70759;,
 -25.25829;123.04398;-19.51282;,
 -31.09763;117.83285;-17.39820;,
 -29.75331;118.43848;-19.39727;,
 -27.71199;118.84436;-20.75316;,
 -25.26030;118.57956;-19.92084;,
 -24.21460;111.65232;38.95158;,
 -27.39449;109.05295;38.99482;,
 -24.18914;108.67107;40.19938;,
 -29.32398;105.56380;37.70893;,
 -27.13444;104.93667;39.72937;,
 -24.18045;104.70964;40.43098;,
 -30.13825;101.51369;35.95710;,
 -28.72291;100.91132;37.90752;,
 -26.63415;100.51012;39.19064;,
 -24.21392;100.78068;38.27248;,
 -29.97472;66.02674;-35.42382;,
 -27.93342;66.43273;-36.77973;,
 -31.31906;65.42118;-33.42477;,
 -25.48173;66.16787;-35.94742;,
 -30.36191;64.70972;-31.11146;,
 -30.36191;64.70972;-31.11146;,
 -29.48634;49.81556;17.58495;,
 -29.26489;102.22722;33.61151;,
 -30.14048;117.12143;-15.08487;,
 -29.48634;49.81556;17.58495;,
 -30.35968;49.10201;19.93056;,
 -28.94435;48.49962;21.88099;,
 -26.85559;48.09847;23.16408;,
 -24.43536;48.36900;22.24593;,
 1.02606;48.39862;21.79717;,
 1.24750;100.81039;37.82374;,
 -24.21392;100.78068;38.27248;,
 -24.43536;48.36900;22.24593;,
 1.24750;100.81039;37.82374;,
 1.28096;104.73932;39.98221;,
 1.27227;108.70074;39.75064;,
 1.24680;111.68195;38.50282;,
 1.20845;113.22928;36.42861;,
 0.28743;128.00110;-14.85246;,
 0.24188;126.22786;-17.55414;,
 0.20313;123.07364;-19.96155;,
 0.20113;118.60923;-20.36958;,
 0.37482;128.12235;-12.26950;,
 -25.48173;66.16787;-35.94742;,
 -19.41734;88.97896;-29.07669;,
 -13.80561;85.87294;-30.12758;,
 -0.02030;66.19754;-36.39616;,
 -21.71546;96.49063;-26.73785;,
 -19.35384;104.00770;-24.48113;,
 -13.71581;107.12685;-23.62851;,
 -8.10408;104.02081;-24.67940;,
 -8.16757;88.99205;-29.27496;,
 -5.80596;96.50911;-27.01822;,
 29.10229;112.42698;35.68167;,
 28.22669;127.32110;-13.01470;,
 25.79425;128.15316;-12.71648;,
 26.66984;113.25890;35.97988;,
 31.15727;110.05213;34.91836;,
 30.28168;124.94627;-13.77801;,
 32.52205;106.49586;33.80611;,
 31.64649;121.39004;-14.89028;,
 31.58909;102.29809;32.53897;,
 30.71348;117.19234;-16.15739;,
 25.74883;128.03076;-15.30122;,
 28.90864;125.87562;-16.79831;,
 25.70327;126.25752;-18.00288;,
 30.80803;122.24916;-17.68825;,
 28.61852;122.87621;-19.70873;,
 25.66451;123.10326;-20.41033;,
 31.58685;117.90576;-18.50298;,
 30.17151;118.50820;-20.45340;,
 28.08277;118.90943;-21.73652;,
 25.66253;118.63883;-20.81833;,
 26.70818;111.71161;38.05407;,
 29.89366;109.11970;37.98514;,
 26.73364;108.73044;39.30189;,
 31.78478;105.63492;36.63189;,
 29.66926;105.00278;38.72821;,
 26.74235;104.76896;39.53346;,
 32.54623;101.58664;34.85231;,
 31.20190;100.98110;36.85137;,
 29.16060;100.57517;38.20727;,
 26.70889;100.84003;37.37499;,
 27.86133;66.49769;-37.76310;,
 29.95008;66.09650;-36.48000;,
 31.36542;65.49413;-34.52959;,
 25.44110;66.22717;-36.84495;,
 30.49206;64.78058;-32.18397;,
 30.71348;117.19234;-16.15739;,
 31.58909;102.29809;32.53897;,
 31.36765;49.88642;16.51243;,
 30.49206;64.78058;-32.18397;,
 32.32480;49.17499;18.82576;,
 31.36765;49.88642;16.51243;,
 30.98047;48.56940;20.82484;,
 28.93917;48.16341;22.18074;,
 26.48746;48.42826;21.34843;,
 26.48746;48.42826;21.34843;,
 26.70889;100.84003;37.37499;,
 0.29084;128.12223;-12.26803;,
 13.93149;85.90526;-30.61641;,
 19.56952;89.02436;-29.76378;,
 25.44110;66.22717;-36.84495;,
 21.93114;96.54143;-27.50710;,
 19.63302;104.05314;-25.16827;,
 14.02129;107.15912;-24.11734;,
 8.38326;104.04002;-24.97000;,
 8.31976;89.01127;-29.56551;,
 6.02164;96.52289;-27.22671;,
 -13.76071;96.49984;-26.87804;,
 -13.71581;107.12685;-23.62851;,
 -8.10408;104.02081;-24.67940;,
 -5.80596;96.50911;-27.01822;,
 -8.16757;88.99205;-29.27496;,
 -13.80561;85.87294;-30.12758;,
 -19.41734;88.97896;-29.07669;,
 -21.71546;96.49063;-26.73785;,
 -19.35384;104.00770;-24.48113;,
 8.38326;104.04002;-24.97000;,
 14.02129;107.15912;-24.11734;,
 13.97639;96.53216;-27.36688;,
 6.02164;96.52289;-27.22671;,
 8.31976;89.01127;-29.56551;,
 13.93149;85.90526;-30.61641;,
 19.56952;89.02436;-29.76378;,
 21.93114;96.54143;-27.50710;,
 19.63302;104.05314;-25.16827;,
 -25.36417;-0.02744;-33.65193;,
 -24.46668;-0.03103;17.27091;,
 -26.90366;0.83899;17.31391;,
 -27.80118;0.84262;-33.60896;,
 -28.97158;3.32256;17.35055;,
 -29.86908;3.32620;-33.57226;,
 -30.35573;7.04137;17.37523;,
 -31.25324;7.04501;-33.54762;,
 -29.44561;11.43102;17.35950;,
 -30.34310;11.43468;-33.56334;,
 -25.40812;0.84560;-36.08784;,
 -28.60687;3.32793;-36.77823;,
 -25.44741;3.33177;-38.15264;,
 -30.51721;7.04615;-36.50200;,
 -28.40264;7.04880;-38.69248;,
 -25.47607;7.05232;-39.53215;,
 -31.30129;11.43381;-35.98320;,
 -29.95784;11.43552;-38.07252;,
 -27.91714;11.43803;-39.48882;,
 -25.46503;11.44082;-38.61645;,
 -24.42476;0.84168;19.70695;,
 -27.59717;3.32386;20.51000;,
 -24.39127;3.32743;21.77223;,
 -29.51606;7.04208;20.30178;,
 -27.32561;7.04446;22.41637;,
 -24.37127;7.04779;23.15238;,
 -30.31792;11.42978;19.81159;,
 -28.90169;11.43121;21.85230;,
 -26.81234;11.43357;23.19572;,
 -24.39249;11.43642;22.23758;,
 -30.00079;48.37135;-38.06920;,
 -27.96007;48.37385;-39.48536;,
 -31.34425;48.36961;-35.97981;,
 -25.50798;48.37667;-38.61310;,
 -30.38605;48.37052;-33.56000;,
 -30.38605;48.37052;-33.56000;,
 -30.34310;11.43468;-33.56334;,
 -29.48854;48.36684;17.36285;,
 -29.48854;48.36684;17.36285;,
 -30.36087;48.36561;19.81495;,
 -28.94435;48.49962;21.88099;,
 -26.85559;48.09847;23.16408;,
 -24.43536;48.36900;22.24593;,
 0.05329;0.87525;-36.53659;,
 0.09724;0.00221;-34.10071;,
 0.01400;3.36140;-38.60139;,
 0.09724;0.00221;-34.10071;,
 0.99474;-0.00140;16.82216;,
 -24.46668;-0.03103;17.27091;,
 -0.01464;7.08195;-39.98088;,
 0.99474;-0.00140;16.82216;,
 1.03666;0.87133;19.25819;,
 -0.00361;11.47045;-39.06520;,
 1.07016;3.35711;21.32347;,
 -0.00361;11.47045;-39.06520;,
 -25.46503;11.44082;-38.61645;,
 -25.50798;48.37667;-38.61310;,
 -0.04656;48.40631;-39.06183;,
 1.09015;7.07743;22.70364;,
 1.06892;11.46609;21.78882;,
 1.06892;11.46609;21.78882;,
 1.02606;48.39862;21.79717;,
 -24.43536;48.36900;22.24593;,
 27.99357;0.90757;-34.59228;,
 28.89108;0.90397;16.33055;,
 26.45613;0.02824;16.37342;,
 25.55862;0.03184;-34.54944;,
 30.05572;3.39604;-34.62844;,
 30.95323;3.39234;16.29440;,
 31.43124;7.11800;-34.65243;,
 32.32874;7.11432;16.27042;,
 30.51088;11.50552;-34.63586;,
 31.40837;11.50187;16.28697;,
 25.51468;0.90491;-36.98531;,
 28.68132;3.39466;-37.78789;,
 25.47537;3.39103;-39.05013;,
 30.59154;7.11733;-37.57903;,
 28.40107;7.11498;-39.69362;,
 25.44675;7.11162;-40.42964;,
 31.38320;11.50676;-37.08804;,
 29.96698;11.50528;-39.12870;,
 27.87761;11.50298;-40.47215;,
 25.45778;11.50011;-39.51393;,
 26.49805;0.90100;18.80945;,
 29.69101;3.39057;19.50033;,
 26.53157;3.38673;20.87472;,
 31.59269;7.11324;19.22476;,
 29.47812;7.11059;21.41521;,
 26.55154;7.10707;22.25489;,
 32.36656;11.50277;18.70679;,
 31.02313;11.50097;20.79616;,
 28.98240;11.49855;22.21237;,
 26.53031;11.49571;21.34007;,
 27.83465;48.43880;-40.46872;,
 29.92402;48.44112;-39.12535;,
 31.34024;48.44261;-37.08460;,
 25.41483;48.43595;-39.51056;,
 30.46795;48.44138;-34.63251;,
 31.36544;48.43771;16.29033;,
 30.51088;11.50552;-34.63586;,
 30.46795;48.44138;-34.63251;,
 32.32359;48.43860;18.71015;,
 31.36544;48.43771;16.29033;,
 30.98047;48.56940;20.82484;,
 28.93917;48.16341;22.18074;,
 26.48746;48.42826;21.34843;,
 26.45613;0.02824;16.37342;,
 25.41483;48.43595;-39.51056;,
 25.45778;11.50011;-39.51393;,
 26.48746;48.42826;21.34843;;
 
 290;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 3;10,2,1;,
 3;11,4,2;,
 3;2,10,11;,
 3;12,11,10;,
 3;13,6,4;,
 3;4,11,13;,
 3;14,13,11;,
 3;11,12,14;,
 3;15,14,12;,
 3;16,8,6;,
 3;6,13,16;,
 3;17,16,13;,
 3;13,14,17;,
 3;18,17,14;,
 3;14,15,18;,
 3;19,18,15;,
 3;0,3,20;,
 3;3,5,21;,
 3;21,20,3;,
 3;20,21,22;,
 3;5,7,23;,
 3;23,21,5;,
 3;21,23,24;,
 3;24,22,21;,
 3;22,24,25;,
 3;7,9,26;,
 3;26,23,7;,
 3;23,26,27;,
 3;27,24,23;,
 3;24,27,28;,
 3;28,25,24;,
 3;25,28,29;,
 4;30,17,18,31;,
 4;30,32,16,17;,
 4;31,18,19,33;,
 4;32,34,8,16;,
 4;35,36,37,38;,
 4;39,40,26,9;,
 4;40,41,27,26;,
 4;41,42,28,27;,
 4;42,43,29,28;,
 4;44,45,46,47;,
 4;48,49,25,29;,
 4;49,50,22,25;,
 4;50,51,20,22;,
 4;51,52,0,20;,
 4;53,54,12,10;,
 4;54,55,15,12;,
 4;55,56,19,15;,
 4;57,53,10,1;,
 4;0,52,57,1;,
 4;58,59,60,61;,
 3;59,58,62;,
 3;19,62,58;,
 3;19,63,62;,
 4;56,64,63,19;,
 3;65,64,56;,
 3;66,61,60;,
 3;67,61,66;,
 3;56,67,65;,
 3;61,67,56;,
 4;33,61,44,43;,
 4;34,33,43,36;,
 3;30,34,32;,
 3;31,33,30;,
 3;33,34,30;,
 3;42,41,43;,
 3;41,40,36;,
 3;36,43,41;,
 4;68,69,70,71;,
 4;72,73,69,68;,
 4;74,75,73,72;,
 4;76,77,75,74;,
 3;70,69,78;,
 3;69,73,79;,
 3;79,78,69;,
 3;78,79,80;,
 3;73,75,81;,
 3;81,79,73;,
 3;79,81,82;,
 3;82,80,79;,
 3;80,82,83;,
 3;75,77,84;,
 3;84,81,75;,
 3;81,84,85;,
 3;85,82,81;,
 3;82,85,86;,
 3;86,83,82;,
 3;83,86,87;,
 3;88,68,71;,
 3;89,72,68;,
 3;68,88,89;,
 3;90,89,88;,
 3;91,74,72;,
 3;72,89,91;,
 3;92,91,89;,
 3;89,90,92;,
 3;93,92,90;,
 3;94,76,74;,
 3;74,91,94;,
 3;95,94,91;,
 3;91,92,95;,
 3;96,95,92;,
 3;92,93,96;,
 3;97,96,93;,
 4;98,86,85,99;,
 4;85,84,100,99;,
 4;101,87,86,98;,
 4;84,77,102,100;,
 4;103,104,105,106;,
 4;76,94,107,108;,
 4;94,95,109,107;,
 4;95,96,110,109;,
 4;96,97,111,110;,
 4;112,113,45,44;,
 4;97,93,49,48;,
 4;93,90,50,49;,
 4;90,88,51,50;,
 4;88,71,52,51;,
 4;78,80,54,53;,
 4;80,83,55,54;,
 4;83,87,56,55;,
 4;70,78,53,114;,
 4;70,114,52,71;,
 4;61,115,116,117;,
 3;118,117,116;,
 3;117,118,87;,
 3;118,119,87;,
 4;87,119,120,56;,
 3;56,120,121;,
 3;115,61,122;,
 3;122,61,123;,
 3;121,123,56;,
 3;56,123,61;,
 4;111,44,61,101;,
 4;105,111,101,102;,
 3;100,102,99;,
 3;99,101,98;,
 3;99,102,101;,
 3;111,109,110;,
 3;105,107,109;,
 3;109,111,105;,
 3;124,125,126;,
 3;124,126,127;,
 3;124,127,128;,
 3;124,128,129;,
 3;124,129,130;,
 3;124,130,131;,
 3;124,131,132;,
 3;124,132,125;,
 3;133,134,135;,
 3;136,133,135;,
 3;137,136,135;,
 3;138,137,135;,
 3;139,138,135;,
 3;140,139,135;,
 3;141,140,135;,
 3;134,141,135;,
 4;142,143,144,145;,
 4;145,144,146,147;,
 4;147,146,148,149;,
 4;149,148,150,151;,
 3;142,145,152;,
 3;145,147,153;,
 3;153,152,145;,
 3;152,153,154;,
 3;147,149,155;,
 3;155,153,147;,
 3;153,155,156;,
 3;156,154,153;,
 3;154,156,157;,
 3;149,151,158;,
 3;158,155,149;,
 3;155,158,159;,
 3;159,156,155;,
 3;156,159,160;,
 3;160,157,156;,
 3;157,160,161;,
 3;162,144,143;,
 3;163,146,144;,
 3;144,162,163;,
 3;164,163,162;,
 3;165,148,146;,
 3;146,163,165;,
 3;166,165,163;,
 3;163,164,166;,
 3;167,166,164;,
 3;168,150,148;,
 3;148,165,168;,
 3;169,168,165;,
 3;165,166,169;,
 3;170,169,166;,
 3;166,167,170;,
 3;171,170,167;,
 4;172,173,160,159;,
 4;172,159,158,174;,
 4;173,175,161,160;,
 4;174,158,151,176;,
 4;177,178,150,179;,
 4;180,150,168,181;,
 4;181,168,169,182;,
 4;182,169,170,183;,
 4;183,170,171,184;,
 4;185,186,142,152;,
 4;185,152,154,187;,
 4;188,189,190,142;,
 4;187,154,157,191;,
 4;192,193,162,143;,
 4;191,157,161,194;,
 4;193,195,164,162;,
 4;196,197,198,199;,
 4;195,200,167,164;,
 4;200,201,171,167;,
 4;202,203,204,171;,
 4;175,184,203,199;,
 4;179,184,175,176;,
 3;172,174,176;,
 3;175,173,172;,
 3;176,175,172;,
 3;184,182,183;,
 3;182,179,181;,
 3;184,179,182;,
 4;205,206,207,208;,
 4;209,210,206,205;,
 4;211,212,210,209;,
 4;213,214,212,211;,
 3;215,205,208;,
 3;216,209,205;,
 3;205,215,216;,
 3;217,216,215;,
 3;218,211,209;,
 3;209,216,218;,
 3;219,218,216;,
 3;216,217,219;,
 3;220,219,217;,
 3;221,213,211;,
 3;211,218,221;,
 3;222,221,218;,
 3;218,219,222;,
 3;223,222,219;,
 3;219,220,223;,
 3;224,223,220;,
 3;207,206,225;,
 3;206,210,226;,
 3;226,225,206;,
 3;225,226,227;,
 3;210,212,228;,
 3;228,226,210;,
 3;226,228,229;,
 3;229,227,226;,
 3;227,229,230;,
 3;212,214,231;,
 3;231,228,212;,
 3;228,231,232;,
 3;232,229,228;,
 3;229,232,233;,
 3;233,230,229;,
 3;230,233,234;,
 4;222,223,235,236;,
 4;237,221,222,236;,
 4;223,224,238,235;,
 4;239,213,221,237;,
 4;240,214,241,242;,
 4;243,231,214,244;,
 4;245,232,231,243;,
 4;246,233,232,245;,
 4;247,234,233,246;,
 4;215,208,186,185;,
 4;187,217,215,185;,
 4;208,248,189,188;,
 4;191,220,217,187;,
 4;207,225,193,192;,
 4;194,224,220,191;,
 4;225,227,195,193;,
 4;199,249,250,196;,
 4;227,230,200,195;,
 4;230,234,201,200;,
 4;234,251,203,202;,
 4;199,203,247,238;,
 4;239,238,247,240;,
 3;239,237,236;,
 3;236,235,238;,
 3;236,238,239;,
 3;246,245,247;,
 3;243,240,245;,
 3;245,240,247;;
 
 MeshMaterialList {
  2;
  290;
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
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\AirplaneEye.png";
   }
  }
  Material {
   0.615200;0.615200;0.615200;1.000000;;
   5.000000;
   0.400000;0.400000;0.400000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  261;
  -0.162133;0.878955;0.448498;,
  -0.167936;0.977792;0.125382;,
  0.101205;0.225526;-0.968966;,
  0.194328;-0.382690;0.903208;,
  -0.982154;-0.045751;-0.182431;,
  -0.976035;-0.149821;0.157829;,
  -0.235642;0.928017;-0.288542;,
  -0.196262;0.709399;-0.676931;,
  -0.049474;0.378379;-0.924328;,
  -0.216689;0.605516;0.765765;,
  -0.166754;0.207523;0.963912;,
  -0.015255;-0.203736;0.978907;,
  -0.544091;0.699285;0.463643;,
  -0.551976;0.833489;0.024876;,
  -0.852843;0.404019;0.330800;,
  -0.859150;0.511331;-0.020050;,
  -0.994193;0.076626;0.075554;,
  -0.995319;0.095777;0.012950;,
  -0.164794;0.296591;-0.940679;,
  -0.717315;0.268641;-0.642878;,
  -0.991334;0.049636;-0.121628;,
  -0.130797;-0.281838;0.950505;,
  -0.693057;-0.144030;0.706348;,
  -0.986250;-0.036841;0.161104;,
  -0.601687;0.692244;-0.398461;,
  -0.850865;0.373731;-0.369261;,
  -0.490419;0.481425;-0.726443;,
  -0.581269;0.344952;0.736976;,
  -0.834464;0.094750;0.542855;,
  -0.461407;-0.012091;0.887106;,
  -0.719150;0.205964;-0.663628;,
  -0.131630;0.290381;-0.947815;,
  -0.995230;0.032349;-0.092040;,
  0.072119;0.291432;-0.953869;,
  -0.980670;-0.053416;0.188236;,
  -0.986817;0.051124;-0.153555;,
  -0.991290;-0.034663;0.127055;,
  -0.694840;-0.207609;0.688546;,
  -0.097477;-0.290648;0.951852;,
  0.195623;-0.287523;0.937583;,
  -0.017187;0.292509;-0.956108;,
  0.017192;-0.292440;0.956129;,
  0.016687;-0.389047;0.921067;,
  0.017446;-0.220178;0.975304;,
  0.016905;0.225493;0.974098;,
  0.013170;0.615651;0.787909;,
  0.006924;0.892108;0.451770;,
  -0.017343;0.252934;-0.967328;,
  -0.006550;0.951129;-0.308723;,
  -0.012861;0.731834;-0.681361;,
  -0.016842;0.363066;-0.931611;,
  0.001063;0.992217;0.124513;,
  -0.017057;0.292548;-0.956099;,
  -0.017187;0.292564;-0.956092;,
  -0.016789;0.292437;-0.956138;,
  -0.017041;0.292316;-0.956170;,
  -0.017210;0.292286;-0.956176;,
  -0.017335;0.292344;-0.956156;,
  -0.017303;0.292511;-0.956106;,
  -0.017472;0.292437;-0.956125;,
  0.175781;0.879353;0.442537;,
  0.169975;0.978185;0.119429;,
  -0.135811;0.225249;-0.964789;,
  -0.161486;-0.383104;0.909480;,
  0.975219;-0.043470;-0.216931;,
  0.981336;-0.147544;0.123332;,
  0.223174;0.928548;-0.296634;,
  0.170633;0.709820;-0.683403;,
  0.015993;0.378457;-0.925481;,
  0.242122;0.606061;0.757672;,
  0.200134;0.207950;0.957446;,
  0.050215;-0.203662;0.977753;,
  0.558455;0.700575;0.444209;,
  0.550574;0.834768;0.005446;,
  0.863029;0.406015;0.300555;,
  0.856719;0.513326;-0.050293;,
  0.996057;0.078945;0.040472;,
  0.994931;0.098091;-0.022130;,
  0.130863;0.296938;-0.945887;,
  0.693595;0.270282;-0.667738;,
  0.986314;0.051936;-0.156485;,
  0.164866;-0.281494;0.945294;,
  0.717849;-0.142387;0.681483;,
  0.991398;-0.034537;0.126247;,
  0.585662;0.693620;-0.419393;,
  0.836453;0.375691;-0.399003;,
  0.463401;0.482534;-0.743250;,
  0.606074;0.346337;0.716048;,
  0.852854;0.096714;0.513115;,
  0.492406;-0.010984;0.870297;,
  0.694842;0.207609;-0.688544;,
  0.097478;0.290648;-0.951852;,
  0.991290;0.034663;-0.127053;,
  -0.106360;0.291213;-0.950727;,
  0.986816;-0.051125;0.153558;,
  0.980671;0.053415;-0.188234;,
  0.995229;-0.032350;0.092043;,
  0.719152;-0.205963;0.663626;,
  0.131636;-0.290380;0.947814;,
  -0.161792;-0.287938;0.943883;,
  0.001013;0.992217;0.124518;,
  -0.017314;0.292525;-0.956101;,
  -0.017179;0.292541;-0.956099;,
  -0.017596;0.292435;-0.956123;,
  -0.017341;0.292316;-0.956165;,
  -0.017173;0.292289;-0.956176;,
  -0.017046;0.292347;-0.956160;,
  -0.017074;0.292510;-0.956110;,
  -0.016909;0.292437;-0.956135;,
  -0.004038;-0.956283;-0.292414;,
  -0.004041;-0.956283;-0.292414;,
  -0.004037;-0.956284;-0.292412;,
  -0.004048;-0.956283;-0.292416;,
  -0.004040;-0.956283;-0.292414;,
  -0.004049;-0.956283;-0.292416;,
  -0.004071;-0.956282;-0.292419;,
  -0.004038;-0.956284;-0.292413;,
  -0.004036;-0.956286;-0.292406;,
  -0.004040;-0.956284;-0.292412;,
  -0.004048;-0.956283;-0.292414;,
  -0.004038;-0.956283;-0.292415;,
  -0.004042;-0.956283;-0.292414;,
  -0.004043;-0.956283;-0.292414;,
  -0.004041;-0.956284;-0.292412;,
  -0.004038;-0.956284;-0.292411;,
  -0.004043;-0.956284;-0.292413;,
  -0.004044;-0.956282;-0.292417;,
  -0.004042;-0.956283;-0.292415;,
  -0.004037;-0.956283;-0.292416;,
  -0.004054;-0.956283;-0.292415;,
  -0.017192;0.292438;-0.956130;,
  -0.017190;0.292438;-0.956130;,
  -0.017191;0.292442;-0.956129;,
  -0.017193;0.292444;-0.956128;,
  -0.017192;0.292443;-0.956129;,
  -0.017189;0.292442;-0.956129;,
  -0.017188;0.292442;-0.956129;,
  -0.017192;0.292440;-0.956129;,
  -0.017191;0.292441;-0.956129;,
  -0.017192;0.292440;-0.956129;,
  -0.017187;0.292440;-0.956130;,
  -0.017189;0.292439;-0.956130;,
  -0.017190;0.292440;-0.956129;,
  -0.017192;0.292441;-0.956129;,
  -0.017194;0.292440;-0.956129;,
  -0.017195;0.292439;-0.956130;,
  -0.017197;0.292440;-0.956129;,
  -0.017192;0.292440;-0.956129;,
  0.160545;0.101344;-0.981812;,
  -0.170792;-0.971214;-0.166053;,
  -0.164837;-0.971237;0.171837;,
  0.193252;0.101224;0.975914;,
  -0.981939;0.099933;-0.160650;,
  -0.975668;0.099909;0.195167;,
  -0.238187;-0.802329;-0.547298;,
  -0.198003;-0.479790;-0.854749;,
  -0.050160;-0.091321;-0.994557;,
  -0.218755;-0.802411;0.555232;,
  -0.167768;-0.479917;0.861123;,
  -0.015088;-0.091465;0.995694;,
  -0.546288;-0.802761;0.239048;,
  -0.554379;-0.802726;-0.219759;,
  -0.854145;-0.480651;0.198521;,
  -0.860614;-0.480622;-0.168363;,
  -0.994442;-0.092519;0.050262;,
  -0.995595;-0.092514;-0.015202;,
  -0.165249;-0.007992;-0.986219;,
  -0.717814;-0.066778;-0.693025;,
  -0.991420;-0.009035;-0.130405;,
  -0.129757;-0.008014;0.991513;,
  -0.691880;-0.066932;0.718903;,
  -0.986301;-0.009097;0.164703;,
  -0.603511;-0.543673;-0.583261;,
  -0.851781;-0.246923;-0.462059;,
  -0.491494;-0.246471;-0.835276;,
  -0.582582;-0.543763;0.604086;,
  -0.834969;-0.246995;0.491753;,
  -0.461751;-0.246590;0.852044;,
  -0.719455;-0.000773;-0.694538;,
  -0.132062;-0.000062;-0.991241;,
  -0.995279;-0.001149;-0.097051;,
  0.161371;0.000278;-0.986894;,
  -0.980593;-0.001158;0.196049;,
  -0.986893;-0.001133;-0.161372;,
  -0.991425;-0.001270;0.130673;,
  -0.691873;-0.000911;0.722019;,
  -0.095581;0.000074;0.995422;,
  0.192498;0.000260;0.981297;,
  -0.009154;-0.819153;-0.573502;,
  -0.001871;-0.985229;-0.171229;,
  -0.014674;-0.500476;-0.865626;,
  0.004162;-0.985253;0.171056;,
  -0.017486;-0.074638;-0.997057;,
  0.011059;-0.819237;0.573349;,
  -0.017648;0.102712;-0.994555;,
  0.015838;-0.500609;0.865529;,
  -0.017622;0.000071;-0.999845;,
  0.017660;-0.074787;0.997043;,
  0.017410;0.102496;0.994581;,
  0.017622;-0.000206;0.999845;,
  -0.195277;0.100930;-0.975541;,
  0.167100;-0.970819;-0.172015;,
  0.173056;-0.970841;0.165889;,
  -0.158982;0.100815;0.982121;,
  0.975433;0.102216;-0.195149;,
  0.981705;0.102190;0.160664;,
  0.220623;-0.801793;-0.555385;,
  0.168881;-0.479364;-0.861214;,
  0.015297;-0.091241;-0.995711;,
  0.240051;-0.801875;0.547149;,
  0.199124;-0.479492;0.854656;,
  0.050379;-0.091390;0.994540;,
  0.556242;-0.801475;0.219619;,
  0.548162;-0.801439;-0.239194;,
  0.861726;-0.478655;0.168281;,
  0.855262;-0.478626;-0.198606;,
  0.995808;-0.090201;0.015189;,
  0.994654;-0.090199;-0.050284;,
  0.130406;-0.007647;-0.991431;,
  0.693095;-0.065139;-0.717897;,
  0.986226;-0.006734;-0.165264;,
  0.164633;-0.007672;0.986325;,
  0.716938;-0.065293;0.694072;,
  0.991511;-0.006792;0.129845;,
  0.583848;-0.542291;-0.604186;,
  0.835539;-0.244962;-0.491801;,
  0.462324;-0.245360;-0.852089;,
  0.604769;-0.542381;0.583161;,
  0.852350;-0.245029;0.462019;,
  0.492062;-0.245480;0.835233;,
  0.694531;0.000874;-0.719462;,
  0.097042;0.000204;-0.995280;,
  0.991240;0.001165;-0.132065;,
  -0.196048;-0.000138;-0.980594;,
  0.986894;0.001132;0.161367;,
  0.980594;0.001158;-0.196047;,
  0.995414;0.001043;0.095655;,
  0.716889;0.000730;0.697187;,
  0.130596;0.000337;0.991436;,
  -0.157803;-0.000148;0.987471;,
  -0.001080;0.999999;0.000104;,
  0.008499;0.997819;0.065466;,
  -0.001161;0.999999;0.000147;,
  -0.010613;0.999776;-0.018293;,
  -0.001047;0.999999;0.000095;,
  -0.001169;0.999999;0.000081;,
  -0.001159;0.999999;0.000086;,
  -0.001175;0.999999;0.000071;,
  -0.009103;0.996741;0.080150;,
  0.004036;0.956286;0.292406;,
  -0.062627;0.997799;-0.021780;,
  -0.008515;0.997799;0.065769;,
  -0.001247;0.999999;0.000105;,
  -0.001284;0.999999;0.000097;,
  0.007635;0.999798;-0.018614;,
  -0.001177;0.999999;0.000084;,
  -0.001160;0.999999;0.000077;,
  -0.001151;0.999999;0.000064;,
  0.004037;0.956283;0.292416;,
  0.009600;0.996763;0.079825;,
  0.059512;0.997941;-0.023930;;
  290;
  4;0,1,13,12;,
  4;12,13,15,14;,
  4;14,15,17,16;,
  4;16,17,5,4;,
  3;6,13,1;,
  3;24,15,13;,
  3;13,6,24;,
  3;7,24,6;,
  3;25,17,15;,
  3;15,24,25;,
  3;26,25,24;,
  3;24,7,26;,
  3;8,26,7;,
  3;20,5,17;,
  3;17,25,20;,
  3;19,20,25;,
  3;25,26,19;,
  3;18,19,26;,
  3;26,8,18;,
  3;2,18,8;,
  3;0,12,9;,
  3;12,14,27;,
  3;27,9,12;,
  3;9,27,10;,
  3;14,16,28;,
  3;28,27,14;,
  3;27,28,29;,
  3;29,10,27;,
  3;10,29,11;,
  3;16,4,23;,
  3;23,28,16;,
  3;28,23,22;,
  3;22,29,28;,
  3;29,22,21;,
  3;21,11,29;,
  3;11,21,3;,
  4;30,19,18,31;,
  4;30,32,20,19;,
  4;31,18,2,33;,
  4;32,34,5,20;,
  4;34,35,4,5;,
  4;35,36,23,4;,
  4;36,37,22,23;,
  4;37,38,21,22;,
  4;38,39,3,21;,
  4;41,42,3,39;,
  4;42,43,11,3;,
  4;43,44,10,11;,
  4;44,45,9,10;,
  4;45,46,0,9;,
  4;48,49,7,6;,
  4;49,50,8,7;,
  4;50,47,2,8;,
  4;51,48,6,1;,
  4;0,46,51,1;,
  4;33,52,53,40;,
  3;52,33,54;,
  3;2,54,33;,
  3;2,55,54;,
  4;47,56,55,2;,
  3;57,56,47;,
  3;58,40,53;,
  3;59,40,58;,
  3;47,59,57;,
  3;40,59,47;,
  4;109,110,110,111;,
  4;112,109,111,113;,
  3;114,112,115;,
  3;116,109,114;,
  3;109,112,114;,
  3;117,118,111;,
  3;118,119,113;,
  3;113,111,118;,
  4;72,73,61,60;,
  4;74,75,73,72;,
  4;76,77,75,74;,
  4;64,65,77,76;,
  3;61,73,66;,
  3;73,75,84;,
  3;84,66,73;,
  3;66,84,67;,
  3;75,77,85;,
  3;85,84,75;,
  3;84,85,86;,
  3;86,67,84;,
  3;67,86,68;,
  3;77,65,80;,
  3;80,85,77;,
  3;85,80,79;,
  3;79,86,85;,
  3;86,79,78;,
  3;78,68,86;,
  3;68,78,62;,
  3;69,72,60;,
  3;87,74,72;,
  3;72,69,87;,
  3;70,87,69;,
  3;88,76,74;,
  3;74,87,88;,
  3;89,88,87;,
  3;87,70,89;,
  3;71,89,70;,
  3;83,64,76;,
  3;76,88,83;,
  3;82,83,88;,
  3;88,89,82;,
  3;81,82,89;,
  3;89,71,81;,
  3;63,81,71;,
  4;91,78,79,90;,
  4;79,80,92,90;,
  4;93,62,78,91;,
  4;80,65,94,92;,
  4;65,64,95,94;,
  4;64,83,96,95;,
  4;83,82,97,96;,
  4;82,81,98,97;,
  4;81,63,99,98;,
  4;99,63,42,41;,
  4;63,71,43,42;,
  4;71,70,44,43;,
  4;70,69,45,44;,
  4;69,60,46,45;,
  4;66,67,49,48;,
  4;67,68,50,49;,
  4;68,62,47,50;,
  4;61,66,48,100;,
  4;61,100,46,60;,
  4;40,102,101,93;,
  3;103,93,101;,
  3;93,103,62;,
  3;103,104,62;,
  4;62,104,105,47;,
  3;47,105,106;,
  3;102,40,107;,
  3;107,40,108;,
  3;106,108,47;,
  3;47,108,40;,
  4;120,110,110,121;,
  4;122,120,121,123;,
  3;124,123,125;,
  3;125,121,126;,
  3;125,123,121;,
  3;120,127,128;,
  3;122,129,127;,
  3;127,120,122;,
  3;138,130,131;,
  3;138,131,132;,
  3;138,132,133;,
  3;138,133,134;,
  3;138,134,135;,
  3;138,135,136;,
  3;138,136,137;,
  3;138,137,130;,
  3;140,139,147;,
  3;141,140,147;,
  3;142,141,147;,
  3;143,142,147;,
  3;144,143,147;,
  3;145,144,147;,
  3;146,145,147;,
  3;139,146,147;,
  4;149,150,160,161;,
  4;161,160,162,163;,
  4;163,162,164,165;,
  4;165,164,152,153;,
  3;149,161,154;,
  3;161,163,172;,
  3;172,154,161;,
  3;154,172,155;,
  3;163,165,173;,
  3;173,172,163;,
  3;172,173,174;,
  3;174,155,172;,
  3;155,174,156;,
  3;165,153,168;,
  3;168,173,165;,
  3;173,168,167;,
  3;167,174,173;,
  3;174,167,166;,
  3;166,156,174;,
  3;156,166,148;,
  3;157,160,150;,
  3;175,162,160;,
  3;160,157,175;,
  3;158,175,157;,
  3;176,164,162;,
  3;162,175,176;,
  3;177,176,175;,
  3;175,158,177;,
  3;159,177,158;,
  3;171,152,164;,
  3;164,176,171;,
  3;170,171,176;,
  3;176,177,170;,
  3;169,170,177;,
  3;177,159,169;,
  3;151,169,159;,
  4;178,179,166,167;,
  4;178,167,168,180;,
  4;179,181,148,166;,
  4;180,168,153,182;,
  4;182,153,152,183;,
  4;183,152,171,184;,
  4;184,171,170,185;,
  4;185,170,169,186;,
  4;186,169,151,187;,
  4;188,189,149,154;,
  4;188,154,155,190;,
  4;189,191,150,149;,
  4;190,155,156,192;,
  4;191,193,157,150;,
  4;192,156,148,194;,
  4;193,195,158,157;,
  4;194,148,181,196;,
  4;195,197,159,158;,
  4;197,198,151,159;,
  4;198,199,187,151;,
  4;240,241,242,242;,
  4;243,241,240,244;,
  3;245,246,244;,
  3;240,247,245;,
  3;244,240,245;,
  3;241,248,249;,
  3;248,243,250;,
  3;241,243,248;,
  4;213,212,202,201;,
  4;215,214,212,213;,
  4;217,216,214,215;,
  4;205,204,216,217;,
  3;206,213,201;,
  3;224,215,213;,
  3;213,206,224;,
  3;207,224,206;,
  3;225,217,215;,
  3;215,224,225;,
  3;226,225,224;,
  3;224,207,226;,
  3;208,226,207;,
  3;220,205,217;,
  3;217,225,220;,
  3;219,220,225;,
  3;225,226,219;,
  3;218,219,226;,
  3;226,208,218;,
  3;200,218,208;,
  3;202,212,209;,
  3;212,214,227;,
  3;227,209,212;,
  3;209,227,210;,
  3;214,216,228;,
  3;228,227,214;,
  3;227,228,229;,
  3;229,210,227;,
  3;210,229,211;,
  3;216,204,223;,
  3;223,228,216;,
  3;228,223,222;,
  3;222,229,228;,
  3;229,222,221;,
  3;221,211,229;,
  3;211,221,203;,
  4;219,218,231,230;,
  4;232,220,219,230;,
  4;218,200,233,231;,
  4;234,205,220,232;,
  4;235,204,205,234;,
  4;236,223,204,235;,
  4;237,222,223,236;,
  4;238,221,222,237;,
  4;239,203,221,238;,
  4;206,201,189,188;,
  4;190,207,206,188;,
  4;201,202,191,189;,
  4;192,208,207,190;,
  4;202,209,193,191;,
  4;194,200,208,192;,
  4;209,210,195,193;,
  4;196,233,200,194;,
  4;210,211,197,195;,
  4;211,203,198,197;,
  4;203,239,199,198;,
  4;242,242,251,252;,
  4;253,252,251,254;,
  3;253,255,256;,
  3;256,257,252;,
  3;256,252,253;,
  3;258,259,251;,
  3;260,254,259;,
  3;259,254,251;;
 }
 MeshTextureCoords {
  252;
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.402600;1.000000;,
  0.402600;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.402600;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.402600;,
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
  -1.415160;0.957460;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.957460;,
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
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.402600;0.000000;,
  0.402600;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.402600;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  -1.415160;0.957460;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  -0.487480;-0.536220;,
  -0.487480;-0.993290;,
  -0.086970;-0.859420;,
  0.078920;-0.536220;,
  -0.086970;-0.213020;,
  -0.487480;-0.079150;,
  -0.887980;-0.213020;,
  -1.053880;-0.536220;,
  -0.887980;-0.859420;,
  1.086970;-0.859420;,
  1.487480;-0.993290;,
  1.487480;-0.536220;,
  0.921080;-0.536220;,
  1.086970;-0.213020;,
  1.487480;-0.079150;,
  1.887980;-0.213020;,
  2.053880;-0.536220;,
  1.887980;-0.859420;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.402600;1.000000;,
  0.000000;1.000000;,
  0.402600;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;2.686980;,
  -1.415160;2.686980;,
  -1.415160;0.957460;,
  0.500000;0.957460;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.402600;,
  0.000000;0.402600;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.402600;0.000000;,
  0.000000;1.000000;,
  0.402600;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  -1.415160;0.957460;,
  -1.415160;2.686980;,
  0.000000;0.402600;;
 }
}
