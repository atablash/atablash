#pragma once


namespace ab{


#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4305) // 'initializing' : truncation from 'double' to 'const float'
#endif

// probix(x), x=0..1
const float probit1024[] = {-3.097558392975658,-2.885942132469943,-2.755868165559522,-2.660396199829992,-2.584356209392189,-2.520845717310525,-2.466120528560782,-2.417914221592359,-2.374747142477102,-2.335598149625034,-2.299731883252602,-2.266600620681125,-2.235785065792777,-2.20695686307142,-2.179853905073575,-2.154263517342249,-2.130010679262959,-2.106949569169807,-2.084957365441463,-2.06392961611032,-2.043776722506634,-2.024421229270474,-2.005795708001292,-1.987841084644705,-1.970505303170329,-1.953742247324262,-1.93751086271024,-1.921774436013785,-1.906499998688552,-1.891657830102795,-1.877221040824709,-1.863165220975591,-1.849468141792318,-1.836109500992796,-1.823070704426331,-1.810334677957496,-1.797885704680065,-1.785709283462875,-1.773792005548417,-1.762121446499574,-1.75068607125217,-1.739475150404887,-1.728478686182375,-1.717687346756155,-1.707092407812454,-1.696685700425005,-1.686459564430974,-1.676406806624952,-1.666520663183652,-1.656794765815982,-1.647223111202421,-1.637800033346129,-1.628520178507961,-1.619378482439964,-1.610370149668073,-1.601490634605835,-1.592735624307655,-1.584101022693083,-1.575582936093572,-1.567177659990391,-1.558881666827391,-1.550691594795363,-1.542604237496186,-1.534616534404904,-1.526725562056687,-1.518928525893307,-1.51122275271056,-1.503605683654058,-1.496074867716132,-1.488627955691257,-1.481262694551604,-1.473976922208031,-1.466768562625123,-1.459635621261844,-1.452576180812005,-1.445588397221092,-1.438670495958158,-1.431820768523329,-1.425037569173211,-1.418319311848047,-1.411664467285797,-1.405071560309632,-1.398539167276437,-1.392065913674922,-1.385650471862928,-1.379291558934291,-1.37298793470645,-1.366738399820644,-1.360541793947194,-1.354396994088954,-1.348302912976517,-1.342258497549272,-1.336262727516836,-1.330314613995774,-1.324413198216942,-1.318557550299055,-1.312746768084458,-1.306979976033318,-1.301256324172764,-1.295574987097688,-1.289935163020201,-1.284336072864907,-1.278776959407354,-1.273257086453213,-1.267775738055884,-1.262332217770371,-1.256925847941436,-1.251555969024137,-1.246221938934995,-1.240923132432152,-1.235658940522947,-1.230428769897494,-1.225232042386869,-1.220068194444647,-1.214936676650586,-1.209836953235313,-1.204768501624966,-1.199730812004785,-1.194723386900707,-1.189745740778077,-1.184797399656649,-1.179877900741064,-1.17498679206609,-1.170123632155891,-1.165287989696682,-1.160479443222132,-1.155697580810918,-1.150941999795884,-1.146212306484246,-1.141508115888377,-1.136829051466652,-1.132174744873943,-1.127544835721305,-1.12293897134446,-1.118356806580697,-1.113798003553811,-1.109262231466746,-1.104749166401602,-1.1002584911267,-1.095789894910401,-1.091343073341402,-1.086917728155233,-1.082513567066707,-1.078130303608069,-1.073767656972616,-1.069425351863572,-1.065103118347994,-1.060800691715526,-1.056517812341786,-1.052254225556229,-1.048009681514285,-1.043783935073628,-1.039576745674407,-1.035387877223279,-1.031217097981115,-1.02706418045423,-1.022928901289005,-1.018811041169779,-1.014710384719881,-1.010626720405697,-1.006559840443653,-1.002509540710008,-0.9984756206533578,-0.9944578832097531,-0.9904561347203336,-0.9864701848513928,-0.9824998465167862,-0.9785449358025993,-0.974605271893998,-0.970680677004187,-0.966770976305398,-0.9628759978618452,-0.9589955725645742,-0.9551295340681464,-0.9512777187290917,-0.9474399655460722,-0.9436161161017016,-0.9398060145059618,-0.9360095073411675,-0.9322264436084267,-0.9284566746755457,-0.9247000542263371,-0.9209564382112815,-0.9172256847994996,-0.913507654331996,-0.9098022092761303,-0.9061092141812808,-0.9024285356356578,-0.89876004222424,-0.8951036044877863,-0.8914590948829019,-0.8878263877431195,-0.884205359240965,-0.8805958873509807,-0.8769978518136744,-0.8734111341003672,-0.8698356173789145,-0.8662711864802711,-0.8627177278658789,-0.8591751295958504,-0.8556432812979253,-0.8521220741371801,-0.8486114007864618,-0.8451111553975357,-0.8416212335729143,-0.8381415323383598,-0.834671950116031,-0.8312123866982626,-0.8277627432219572,-0.8243229221435711,-0.8208928272146809,-0.8174723634581109,-0.8140614371446091,-0.8106599557700533,-0.8072678280331768,-0.8038849638137954,-0.8005112741515247,-0.7971466712249743,-0.7937910683314048,-0.7904443798668355,-0.7871065213065922,-0.7837774091862812,-0.7804569610831813,-0.7771450955980376,-0.773841732337253,-0.7705467918954627,-0.7672601958384825,-0.7639818666866213,-0.7607117278983514,-0.7574497038543204,-0.7541957198417045,-0.7509497020388882,-0.7477115775004644,-0.7444812741425476,-0.7412587207283911,-0.7380438468543002,-0.7348365829358366,-0.7316368601943033,-0.728444610643505,-0.7252597670767783,-0.7220822630542832,-0.7189120328905484,-0.7157490116422693,-0.7125931350963457,-0.7094443397581606,-0.7063025628400876,-0.7031677422502258,-0.7000398165813566,-0.6969187251001143,-0.693804407736368,-0.6906968050728098,-0.6875958583347421,-0.6845015093800632,-0.6814137006894415,-0.6783323753566807,-0.6752574770792632,-0.6721889501490763,-0.6691267394433105,-0.6660707904155284,-0.6630210490869014,-0.6599774620376083,-0.6569399763983937,-0.6539085398422818,-0.6508831005764422,-0.6478636073342042,-0.6448500093672185,-0.6418422564377582,-0.6388402988111609,-0.6358440872484065,-0.6328535729988283,-0.629868707792953,-0.6268894438354699,-0.6239157337983234,-0.6209475308139284,-0.6179847884685036,-0.6150274607955232,-0.6120755022692814,-0.6091288677985706,-0.606187512720466,-0.6032513927942216,-0.6003204641952662,-0.5973946835093068,-0.5944740077265291,-0.5915583942358984,-0.5886478008195554,-0.5857421856473083,-0.5828415072712162,-0.5799457246202626,-0.5770547969951191,-0.574168684062994,-0.5712873458525689,-0.5684107427490159,-0.5655388354890976,-0.5626715851563479,-0.5598089531763306,-0.5569509013119746,-0.5540973916589849,-0.5512483866413277,-0.5484038490067875,-0.5455637418225955,-0.5427280284711268,-0.5398966726456673,-0.5370696383462461,-0.5342468898755339,-0.5314283918348061,-0.5286141091199686,-0.5258040069176455,-0.522998050701327,-0.520196206227578,-0.5173984395323037,-0.514604716927073,-0.5118150049954981,-0.5090292705896695,-0.5062474808266438,-0.5034696030849859,-0.5006956050013623,-0.4979254544671863,-0.4951591196253119,-0.4923965688667783,-0.4896377708276017,-0.486882694385615,-0.4841313086573533,-0.4813835829949856,-0.4786394869832911,-0.4758989904366788,-0.4731620633962519,-0.4704286761269123,-0.4676987991145082,-0.4649724030630226,-0.4622494588918005,-0.459529937732817,-0.4568138109279832,-0.45410105002649,-0.4513916267821899,-0.4486855131510141,-0.4459826812884273,-0.4432831035469157,-0.4405867524735119,-0.4378936008073521,-0.4352036214772675,-0.4325167875994089,-0.4298330724749025,-0.4271524495875386,-0.4244748926014909,-0.4218003753590663,-0.4191288718784854,-0.4164603563516914,-0.4137948031421897,-0.4111321867829144,-0.4084724819741238,-0.4058156635813236,-0.4031617066332163,-0.4005105863196786,-0.3978622779897626,-0.3952167571497264,-0.3925739994610858,-0.3899339807386945,-0.3872966769488461,-0.3846620642074017,-0.3820301187779403,-0.3794008170699326,-0.3767741356369381,-0.3741500511748235,-0.3715285405200044,-0.3689095806477081,-0.3662931486702574,-0.3636792218353759,-0.3610677775245139,-0.3584587932511938,-0.3558522466593766,-0.3532481155218471,-0.3506463777386185,-0.3480470113353564,-0.3454499944618208,-0.342855305390327,-0.340262922514224,-0.3376728243463912,-0.3350849895177517,-0.3324993967758037,-0.3299160249831679,-0.3273348531161523,-0.3247558602633323,-0.3221790256241478,-0.3196043285075156,-0.3170317483304572,-0.3144612646167421,-0.3118928569955463,-0.3093265052001247,-0.3067621890664992,-0.3041998885321602,-0.3016395836347824,-0.2990812545109546,-0.2965248813949227,-0.2939704446173466,-0.2914179246040696,-0.2888673018749015,-0.2863185570424135,-0.2837716708107464,-0.2812266239744309,-0.2786833974172194,-0.2761419721109309,-0.2736023291143065,-0.2710644495718769,-0.2685283147128412,-0.2659939058499566,-0.2634612043784398,-0.2609301917748781,-0.2584008495961521,-0.2558731594783677,-0.2533471031357998,-0.2508226623598447,-0.2482998190179835,-0.2457785550527543,-0.2432588524807349,-0.2407406933915342,-0.2382240599467932,-0.2357089343791951,-0.2331952989914843,-0.2306831361554945,-0.2281724283111849,-0.2256631579656855,-0.2231553076923502,-0.2206488601298189,-0.2181437979810866,-0.2156401040125819,-0.2131377610532517,-0.2106367519936553,-0.208137059785064,-0.2056386674385705,-0.2031415580242036,-0.2006457146700509,-0.1981511205613888,-0.1956577589398188,-0.1931656131024111,-0.1906746664008545,-0.188184902240613,-0.1856963040800893,-0.1832088554297933,-0.1807225398515188,-0.1782373409575242,-0.1757532424097207,-0.1732702279188659,-0.1707882812437627,-0.1683073861904646,-0.1658275266114864,-0.1633486864050197,-0.1608708495141542,-0.1583939999261051,-0.1559181216714435,-0.1534431988233343,-0.1509692154967772,-0.1484961558478536,-0.1460240040729773,-0.1435527444081512,-0.1410823611282271,-0.138612838546171,-0.1361441610123323,-0.1336763129137173,-0.1312092786732673,-0.1287430427491399,-0.1262775896339953,-0.1238129038542862,-0.1213489699695508,-0.1188857725717109,-0.1164232962843726,-0.1139615257621307,-0.1115004456898773,-0.1090400407821129,-0.1065802957822614,-0.1041211954619883,-0.1016627246205216,-0.09920486808397644,-0.09674761070468241,-0.09429093736051368,-0.09183483295422223,-0.08937928241277357,-0.0869242706866855,-0.0844697827493691,-0.08201580359647261,-0.0795623182452275,-0.07710931173379722,-0.07465676912062751,-0.0722046754838004,-0.06975301592038885,-0.06730177554581435,-0.06485093949320622,-0.06240049291276312,-0.05995042097111603,-0.05750070885069351,-0.05505134174908826,-0.05260230487842563,-0.05015358346473367,-0.0477051627473145,-0.04525702797811745,-0.04280916442111332,-0.0403615573516701,-0.03791419205592982,-0.03546705383018676,-0.03302012798026652,-0.0305733998209063,-0.02812685467513613,-0.02568047787366096,-0.02323425475424357,-0.02078817066108835,-0.01834221094422562,-0.01589636095889674,-0.01345060606493972,-0.01100493162617527,-0.008559323009793377,-0.006113765585740239,-0.003668244726105415,-0.001222745804509297,0.001222745804509297,0.003668244726105415,0.006113765585740239,0.008559323009793377,0.01100493162617527,0.01345060606493972,0.01589636095889674,0.01834221094422562,0.02078817066108835,0.02323425475424357,0.02568047787366096,0.02812685467513613,0.0305733998209063,0.03302012798026652,0.03546705383018676,0.03791419205592982,0.0403615573516701,0.04280916442111332,0.04525702797811745,0.0477051627473145,0.05015358346473367,0.05260230487842563,0.05505134174908826,0.05750070885069351,0.05995042097111603,0.06240049291276312,0.06485093949320622,0.06730177554581435,0.06975301592038885,0.0722046754838004,0.07465676912062751,0.07710931173379722,0.07956231824522764,0.08201580359647274,0.08446978274936924,0.08692427068668564,0.08937928241277374,0.09183483295422237,0.09429093736051382,0.09674761070468255,0.09920486808397658,0.1016627246205217,0.1041211954619884,0.1065802957822616,0.1090400407821131,0.1115004456898775,0.1139615257621309,0.1164232962843727,0.118885772571711,0.1213489699695509,0.1238129038542863,0.1262775896339955,0.12874304274914,0.1312092786732674,0.1336763129137175,0.1361441610123324,0.1386128385461711,0.1410823611282272,0.1435527444081513,0.1460240040729775,0.1484961558478537,0.1509692154967774,0.1534431988233345,0.1559181216714436,0.1583939999261049,0.1608708495141541,0.1633486864050195,0.1658275266114863,0.1683073861904645,0.1707882812437625,0.1732702279188657,0.1757532424097206,0.1782373409575241,0.1807225398515187,0.1832088554297932,0.1856963040800892,0.1881849022406129,0.1906746664008543,0.1931656131024109,0.1956577589398187,0.1981511205613886,0.2006457146700508,0.2031415580242035,0.2056386674385704,0.2081370597850639,0.2106367519936551,0.2131377610532516,0.2156401040125817,0.2181437979810865,0.2206488601298187,0.2231553076923501,0.2256631579656853,0.2281724283111848,0.2306831361554944,0.2331952989914841,0.2357089343791949,0.2382240599467932,0.2407406933915342,0.2432588524807349,0.2457785550527543,0.2482998190179835,0.2508226623598447,0.2533471031357998,0.2558731594783677,0.2584008495961521,0.2609301917748781,0.2634612043784398,0.2659939058499566,0.2685283147128412,0.2710644495718769,0.2736023291143065,0.2761419721109309,0.2786833974172194,0.2812266239744309,0.2837716708107464,0.2863185570424135,0.2888673018749015,0.2914179246040696,0.2939704446173466,0.2965248813949227,0.2990812545109546,0.3016395836347824,0.3041998885321602,0.3067621890664992,0.3093265052001247,0.3118928569955463,0.3144612646167421,0.3170317483304572,0.3196043285075156,0.3221790256241478,0.3247558602633323,0.3273348531161523,0.3299160249831679,0.3324993967758037,0.3350849895177517,0.3376728243463912,0.340262922514224,0.342855305390327,0.3454499944618208,0.3480470113353564,0.3506463777386185,0.3532481155218471,0.3558522466593766,0.3584587932511938,0.3610677775245139,0.3636792218353759,0.3662931486702574,0.3689095806477081,0.3715285405200044,0.3741500511748235,0.3767741356369381,0.3794008170699326,0.3820301187779403,0.3846620642074017,0.3872966769488461,0.3899339807386945,0.3925739994610858,0.3952167571497264,0.3978622779897626,0.4005105863196786,0.4031617066332165,0.4058156635813238,0.4084724819741239,0.4111321867829145,0.4137948031421899,0.4164603563516916,0.4191288718784856,0.4218003753590665,0.424474892601491,0.4271524495875388,0.4298330724749026,0.4325167875994091,0.4352036214772677,0.4378936008073522,0.440586752473512,0.4432831035469159,0.4459826812884274,0.4486855131510143,0.45139162678219,0.4541010500264901,0.4568138109279834,0.4595299377328171,0.4622494588918006,0.4649724030630227,0.4676987991145083,0.4704286761269125,0.4731620633962521,0.4758989904366789,0.4786394869832912,0.4813835829949858,0.4841313086573535,0.4868826943856152,0.4896377708276016,0.4923965688667782,0.4951591196253117,0.4979254544671861,0.5006956050013622,0.5034696030849858,0.5062474808266436,0.5090292705896694,0.511815004995498,0.5146047169270728,0.5173984395323036,0.5201962062275779,0.5229980507013268,0.5258040069176453,0.5286141091199684,0.5314283918348059,0.5342468898755337,0.5370696383462459,0.5398966726456671,0.5427280284711267,0.5455637418225953,0.5484038490067874,0.5512483866413275,0.5540973916589846,0.5569509013119743,0.5598089531763304,0.5626715851563477,0.5655388354890973,0.5684107427490157,0.5712873458525688,0.5741686840629939,0.5770547969951189,0.5799457246202626,0.5828415072712162,0.5857421856473083,0.5886478008195554,0.5915583942358984,0.5944740077265291,0.5973946835093068,0.6003204641952662,0.6032513927942216,0.606187512720466,0.6091288677985706,0.6120755022692814,0.6150274607955232,0.6179847884685036,0.6209475308139284,0.6239157337983234,0.6268894438354699,0.629868707792953,0.6328535729988283,0.6358440872484065,0.6388402988111609,0.6418422564377582,0.6448500093672185,0.6478636073342042,0.6508831005764422,0.6539085398422818,0.6569399763983937,0.6599774620376083,0.6630210490869014,0.6660707904155284,0.6691267394433105,0.6721889501490763,0.6752574770792632,0.6783323753566807,0.6814137006894415,0.6845015093800632,0.6875958583347421,0.6906968050728098,0.693804407736368,0.6969187251001143,0.7000398165813566,0.7031677422502258,0.7063025628400876,0.7094443397581606,0.7125931350963457,0.7157490116422693,0.7189120328905484,0.7220822630542832,0.7252597670767783,0.728444610643505,0.7316368601943033,0.7348365829358366,0.7380438468543002,0.7412587207283911,0.7444812741425476,0.7477115775004644,0.7509497020388882,0.7541957198417045,0.7574497038543204,0.7607117278983514,0.7639818666866213,0.7672601958384825,0.7705467918954627,0.773841732337253,0.7771450955980376,0.7804569610831813,0.7837774091862812,0.7871065213065922,0.7904443798668355,0.7937910683314048,0.7971466712249743,0.8005112741515247,0.8038849638137954,0.8072678280331768,0.8106599557700533,0.8140614371446091,0.8174723634581109,0.8208928272146809,0.8243229221435711,0.8277627432219572,0.8312123866982629,0.8346719501160311,0.83814153233836,0.8416212335729144,0.8451111553975358,0.8486114007864621,0.8521220741371802,0.8556432812979257,0.8591751295958504,0.8627177278658792,0.8662711864802713,0.8698356173789147,0.8734111341003676,0.8769978518136747,0.8805958873509808,0.8842053592409654,0.8878263877431194,0.8914590948829016,0.895103604487786,0.8987600422242398,0.9024285356356577,0.9061092141812802,0.9098022092761301,0.9135076543319959,0.9172256847994994,0.9209564382112811,0.9247000542263368,0.9284566746755454,0.9322264436084264,0.9360095073411674,0.9398060145059614,0.9436161161017015,0.9474399655460722,0.9512777187290917,0.9551295340681464,0.9589955725645742,0.9628759978618452,0.966770976305398,0.970680677004187,0.974605271893998,0.9785449358025993,0.9824998465167862,0.9864701848513928,0.9904561347203336,0.9944578832097531,0.9984756206533578,1.002509540710008,1.006559840443653,1.010626720405697,1.014710384719881,1.018811041169779,1.022928901289005,1.02706418045423,1.031217097981115,1.035387877223279,1.039576745674407,1.043783935073628,1.048009681514285,1.052254225556229,1.056517812341786,1.060800691715526,1.065103118347994,1.069425351863572,1.073767656972616,1.078130303608069,1.082513567066707,1.086917728155233,1.091343073341402,1.095789894910401,1.1002584911267,1.104749166401602,1.109262231466746,1.113798003553811,1.118356806580697,1.12293897134446,1.127544835721305,1.132174744873943,1.136829051466652,1.141508115888377,1.146212306484246,1.150941999795884,1.155697580810918,1.160479443222132,1.165287989696682,1.170123632155891,1.17498679206609,1.179877900741064,1.184797399656649,1.189745740778077,1.194723386900707,1.199730812004785,1.204768501624966,1.209836953235313,1.214936676650586,1.220068194444647,1.225232042386869,1.230428769897494,1.235658940522947,1.240923132432152,1.246221938934995,1.251555969024137,1.256925847941436,1.262332217770371,1.267775738055884,1.273257086453213,1.278776959407354,1.284336072864907,1.289935163020201,1.295574987097688,1.301256324172764,1.306979976033318,1.312746768084458,1.318557550299055,1.324413198216942,1.330314613995774,1.336262727516836,1.342258497549272,1.348302912976517,1.354396994088954,1.360541793947194,1.366738399820644,1.372987934706451,1.379291558934291,1.385650471862928,1.392065913674922,1.398539167276438,1.405071560309633,1.411664467285797,1.418319311848048,1.425037569173212,1.431820768523329,1.438670495958159,1.445588397221092,1.452576180812005,1.459635621261845,1.466768562625123,1.473976922208031,1.481262694551605,1.488627955691258,1.496074867716133,1.503605683654058,1.51122275271056,1.518928525893308,1.526725562056687,1.534616534404903,1.542604237496185,1.550691594795363,1.55888166682739,1.567177659990391,1.575582936093572,1.584101022693083,1.592735624307654,1.601490634605834,1.610370149668072,1.619378482439964,1.628520178507961,1.637800033346128,1.647223111202421,1.656794765815981,1.666520663183651,1.676406806624952,1.686459564430973,1.696685700425005,1.707092407812454,1.717687346756154,1.728478686182374,1.739475150404887,1.75068607125217,1.762121446499574,1.773792005548416,1.785709283462875,1.797885704680064,1.810334677957496,1.823070704426331,1.836109500992796,1.849468141792318,1.863165220975591,1.877221040824709,1.891657830102795,1.906499998688552,1.921774436013785,1.93751086271024,1.953742247324262,1.970505303170329,1.987841084644705,2.005795708001292,2.024421229270474,2.043776722506634,2.06392961611032,2.084957365441463,2.106949569169807,2.130010679262959,2.154263517342249,2.179853905073575,2.20695686307142,2.235785065792777,2.266600620681125,2.299731883252602,2.335598149625034,2.374747142477102,2.417914221592359,2.466120528560782,2.520845717310525,2.584356209392189,2.660396199829992,2.755868165559522,2.885942132469943,3.097558392975658};

#ifdef _MSC_VER
#pragma warning(pop)
#endif


inline float rand_normal(){
	return probit1024[rand()&1023];
}






}

