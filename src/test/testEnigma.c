/**************************************************************************************************\
* 
*  testEnigma.c
*  Test the enigma functions
* 
\**************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "enigma.h"
#include "testframe.h"
#include "toolbox.h"


/**************************************************************************************************\
* Sample texts to be used by various tests
\**************************************************************************************************/
// 1000 chars
char testTextRandom[]="TLTILVLISZYYBNJVXERIOHFHZNCAHXWEMZOYIAODVSTGJVUNYC"
                      "XUJTGQIGJBGLOIJBHXMTTOPCBZMMWIVEUIPRCZDUIGGHOPZSEC"
                      "UDBJNWHLYNMYPVKJSBZNSNDYNVVUWRVRBVHOEVXYBERZTBRPJT"
                      "PCFPFIKFMFIXXFYXWOZZHWSEJIFJRXVGFUDOCPAWBUXYOQZLVT"
                      "SCWLAMRHUZKENQKETAWZFUYXXXDDTGUBDDOPYYTVEOKCGVNVTY"
                      "GLSPPYJXPBLLCNIFOIEREIKGLDGTBKGQNNKIKKKBQZZGKZDHGU"
                      "FEGWLVTRTUHRBDKKOAYYNCJSPFEMBMRGWMBLHESRQPOMCPDNMK"
                      "IUDVKYOFCZGPGKOHUXQRDOKELYARFWIBRGCLHDCXFQOXHSWWDS"
                      "BZNKYYPZCSATQTHCWOGSWNWRRFGBJLORWBYFKLVQNGGSNPUWXC"
                      "YYTOPNLDUDDFMPSOFARDUTRUDAFJGFYOCTXNUVAXIQGRGHSANZ"
                      "MHLFLSFOJRNKGLHPEACGPOTCESQCNNKMYNNKKMCADQJJENBCHC"
                      "QADKGWDEZJASPOTQFJUKMUDEOIJOHHNKOZGGJTHZAZWATWSCAR"
                      "VZUQUJCWIVKTPTBWNKFWFVNBTUKOXGGHCGYYZYQVMVXWJZNGYW"
                      "SEXRLKKEUGEXMCIPADSUBCAQQOTVTMBNOBOPHRPQGTBKUBHLCM"
                      "VTYHNBPORKOCFFEFETFWEWUAKLJKSKRSDMHIJMPBJKZBUAANOS"
                      "DBSJMXNMEJSSDNEXWFHKGHVYEIIXNFYNAWKIKKTCPQVMUWPAFL"
                      "NWEWVIMNNEJXADIHCSQJEHLPBEMGHAGQLCEHFRIOZXWCALIAVX"
                      "FWTXFNFLNQYQGECVQOJKVYNGLJNAVMTNAHQSIUXMBBXQUQGEDK"
                      "DCKPZYHWSSTBJLNJULWOYKALPBTQAEUGVJONKOZNZPQOXKMIDE"
                      "GCETLJVUESNSOSCVVLVAKDCBGAWXBVHMFIABYQRJQHEJYTXMBF";
// 200 chars
char testTextMax[]   ="AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
// 200 chars
char testTextGerman[]="DADIEDEUTSCHENMILITARSEINEWIEDERHOLUNGDERINZWISCHE"
                      "NBEKANNTGEWORDENENKRYPTOGRAPHISCHENKATASTROPHEDESE"
                      "RSTENWELTKRIEGSUNBEDINGTVERMEIDENWOLLTENBETRACHTET"
                      "ENSIEDIENEUEARTDERMASCHINELLENVERSCHLUSSELUNGALSDI";
// 2296+4726 chars from Alice in Wonderland
char testTextEnglish[]=
                    "PRESENTLYSHEBEGANAGAINIWONDERIFISHALLFALLRIGHTTHRO"
                    "UGHTHEEARTHHOWFUNNYITLLSEEMTOCOMEOUTAMONGTHEPEOPLE"
                    "THATWALKWITHTHEIRHEADSDOWNWARDTHEANTIPATHIESITHINK"
                    "SHEWASRATHERGLADTHEREWASNOONELISTENINGTHISTIMEASIT"
                    "DIDNTSOUNDATALLTHERIGHTWORDBUTISHALLHAVETOASKTHEMW"
                    "HATTHENAMEOFTHECOUNTRYISYOUKNOWPLEASEMAAMISTHISNEW"
                    "ZEALANDORAUSTRALIAANDSHETRIEDTOCURTSEYASSHESPOKEFA"
                    "NCYCURTSEYINGASYOUREFALLINGTHROUGHTHEAIRDOYOUTHINK"
                    "YOUCOULDMANAGEITANDWHATANIGNORANTLITTLEGIRLSHELLTH"
                    "INKMEFORASKINGNOITLLNEVERDOTOASKPERHAPSISHALLSEEIT"
                    "WRITTENUPSOMEWHEREDOWNDOWNDOWNTHEREWASNOTHINGELSET"
                    "ODOSOALICESOONBEGANTALKINGAGAINDINAHLLMISSMEVERYMU"
                    "CHTONIGHTISHOULDTHINKDINAHWASTHECATIHOPETHEYLLREME"
                    "MBERHERSAUCEROFMILKATTEATIMEDINAHMYDEARIWISHYOUWER"
                    "EDOWNHEREWITHMETHEREARENOMICEINTHEAIRIMAFRAIDBUTYO"
                    "UMIGHTCATCHABATANDTHATSVERYLIKEAMOUSEYOUKNOWBUTDOC"
                    "ATSEATBATSIWONDERANDHEREALICEBEGANTOGETRATHERSLEEP"
                    "YANDWENTONSAYINGTOHERSELFINADREAMYSORTOFWAYDOCATSE"
                    "ATBATSDOCATSEATBATSANDSOMETIMESDOBATSEATCATSFORYOU"
                    "SEEASSHECOULDNTANSWEREITHERQUESTIONITDIDNTMUCHMATT"
                    "ERWHICHWAYSHEPUTITSHEFELTTHATSHEWASDOZINGOFFANDHAD"
                    "JUSTBEGUNTODREAMTHATSHEWASWALKINGHANDINHANDWITHDIN"
                    "AHANDSAYINGTOHERVERYEARNESTLYNOWDINAHTELLMETHETRUT"
                    "HDIDYOUEVEREATABATWHENSUDDENLYTHUMPTHUMPDOWNSHECAM"
                    "EUPONAHEAPOFSTICKSANDDRYLEAVESANDTHEFALLWASOVERALI"
                    "CEWASNOTABITHURTANDSHEJUMPEDUPONTOHERFEETINAMOMENT"
                    "SHELOOKEDUPBUTITWASALLDARKOVERHEADBEFOREHERWASANOT"
                    "HERLONGPASSAGEANDTHEWHITERABBITWASSTILLINSIGHTHURR"
                    "YINGDOWNITTHEREWASNOTAMOMENTTOBELOSTAWAYWENTALICEL"
                    "IKETHEWINDANDWASJUSTINTIMETOHEARITSAYASITTURNEDACO"
                    "RNEROHMYEARSANDWHISKERSHOWLATEITSGETTINGSHEWASCLOS"
                    "EBEHINDITWHENSHETURNEDTHECORNERBUTTHERABBITWASNOLO"
                    "NGERTOBESEENSHEFOUNDHERSELFINALONGLOWHALLWHICHWASL"
                    "ITUPBYAROWOFLAMPSHANGINGFROMTHEROOFTHEREWEREDOORSA"
                    "LLROUNDTHEHALLBUTTHEYWEREALLLOCKEDANDWHENALICEHADB"
                    "EENALLTHEWAYDOWNONESIDEANDUPTHEOTHERTRYINGEVERYDOO"
                    "RSHEWALKEDSADLYDOWNTHEMIDDLEWONDERINGHOWSHEWASEVER"
                    "TOGETOUTAGAINSUDDENLYSHECAMEUPONALITTLETHREELEGGED"
                    "TABLEALLMADEOFSOLIDGLASSTHEREWASNOTHINGONITEXCEPTA"
                    "TINYGOLDENKEYANDALICESFIRSTTHOUGHTWASTHATITMIGHTBE"
                    "LONGTOONEOFTHEDOORSOFTHEHALLBUTALASEITHERTHELOCKSW"
                    "ERETOOLARGEORTHEKEYWASTOOSMALLBUTATANYRATEITWOULDN"
                    "OTOPENANYOFTHEMHOWEVERONTHESECONDTIMEROUNDSHECAMEU"
                    "PONALOWCURTAINSHEHADNOTNOTICEDBEFOREANDBEHINDITWAS"
                    "ALITTLEDOORABOUTFIFTEENINCHESHIGHSHETRIEDTHELITTLE"
                    "GOLDENKEYINTHELOCKANDTOHERGREATDELIGHTITFITTED"
                    "ITWASTHEWHITERABBITTROTTINGSLOWLYBACKAGAINANDLOOKI"
                    "NGANXIOUSLYABOUTASITWENTASIFITHADLOSTSOMETHINGALIC"
                    "EHEARDITMUTTERINGTOITSELFTHEDUCHESSTHEDUCHESSOHMYD"
                    "EARPAWSOHMYFURANDWHISKERSSHELLGETMEEXECUTEDASSUREA"
                    "SFERRETSAREFERRETSWHERECANIHAVEDROPPEDTHEMIWONDERA"
                    "LICEGUESSEDINAMOMENTTHATITWASLOOKINGFORTHEFANANDTH"
                    "EPAIROFWHITEKIDGLOVESANDSHEVERYGOODNATUREDLYBEGANH"
                    "UNTINGABOUTFORTHEMBUTTHEYWERENOWHERETOBESEENEVERYT"
                    "HINGSEEMEDTOHAVECHANGEDSINCEHERSWIMINTHEPOOLANDTHE"
                    "GREATHALLWITHTHEGLASSTABLEANDTHELITTLEDOORHADVANIS"
                    "HEDCOMPLETELYVERYSOONTHERABBITNOTICEDALICEANDCALLE"
                    "DTOHERINANANGRYTONEWHYMARYANNWHATAREYOUDOINGOUTHER"
                    "ERUNHOMETHISMOMENTANDFETCHMEAPAIROFGLOVESANDAFANQU"
                    "ICKNOWHETOOKMEFORHISHOUSEMAIDSAIDALICEASSHERANOFFH"
                    "OWSURPRISEDHELLBEWHENHEFINDSOUTWHOIAMASSHESAIDTHIS"
                    "SHECAMEUPONANEATLITTLEHOUSEONTHEDOOROFWHICHWASABRI"
                    "GHTBRASSPLATEWITHTHENAMEWRABBITENGRAVEDUPONITSHEWE"
                    "NTINWITHOUTKNOCKINGANDHURRIEDUPSTAIRSINGREATFEARLE"
                    "STSHESHOULDMEETTHEREALMARYANNANDBETURNEDOUTOFTHEHO"
                    "USEBEFORESHEHADFOUNDTHEFANANDGLOVESBYTHISTIMEALICE"
                    "HADFOUNDHERWAYINTOATIDYLITTLEROOMWITHATABLEINTHEWI"
                    "NDOWANDONITAFANANDTWOORTHREEPAIRSOFTINYWHITEKIDGLO"
                    "VESSHETOOKUPTHEFANANDAPAIROFTHEGLOVESANDWASJUSTGOI"
                    "NGTOLEAVETHEROOMWHENHEREYESFELLUPONALITTLEBOTTLETH"
                    "ATSTOODNEARTHELOOKINGGLASSSHEUNCORKEDITANDPUTITTOH"
                    "ERLIPSSAYINGTOHERSELFIDOHOPEITLLMAKEMEGROWLARGEAGA"
                    "INFORREALLYIMQUITETIREDOFBEINGSUCHATINYLITTLETHING"
                    "BEFORESHEHADDRUNKHALFTHEBOTTLESHEFOUNDHERHEADPRESS"
                    "INGAGAINSTTHECEILINGANDHADTOSTOOPTOSAVEHERNECKFROM"
                    "BEINGBROKENSHEHASTILYPUTDOWNTHEBOTTLEREMARKINGTHAT"
                    "SQUITEENOUGHIHOPEISHANTGROWANYMOREALASITWASTOOLATE"
                    "TOWISHTHATSHEWENTONGROWINGANDGROWINGANDVERYSOONSHE"
                    "HADTOKNEELDOWNONTHEFLOORSTILLSHEWENTONGROWINGANDAS"
                    "ALASTRESOURCESHEPUTONEARMOUTOFTHEWINDOWANDONEFOOTU"
                    "PTHECHIMNEYANDSAIDTOHERSELFNOWICANDONOMOREWHATEVER"
                    "HAPPENSWHATWILLBECOMEOFMELUCKILYFORALICETHELITTLEM"
                    "AGICBOTTLEHADNOWHADITSFULLEFFECTANDSHEGREWNOLARGER"
                    "AFTERAFEWMINUTESSHEHEARDAVOICEOUTSIDEANDSTOPPEDTOL"
                    "ISTENMARYANNMARYANNSAIDTHEVOICEFETCHMEMYGLOVESTHIS"
                    "MOMENTTHENCAMEALITTLEPATTERINGOFFEETONTHESTAIRSALI"
                    "CEKNEWITWASTHERABBITCOMINGTOLOOKFORHERANDSHETREMBL"
                    "EDTILLSHESHOOKTHEHOUSEQUITEFORGETTINGTHATSHEWASNOW"
                    "ABOUTATHOUSANDTIMESASLARGEASTHERABBITANDHADNOREASO"
                    "NTOBEAFRAIDOFITPRESENTLYTHERABBITCAMEUPTOTHEDOORAN"
                    "DTRIEDTOOPENITBUTASTHEDOOROPENEDINWARDSANDALICESEL"
                    "BOWWASPRESSEDHARDAGAINSTITTHATATTEMPTPROVEDAFAILUR"
                    "EALICEHEARDITSAYTOITSELFTHENILLGOROUNDANDGETINATTH"
                    "EWINDOWTHATYOUWONTTHOUGHTALICEANDAFTERWAITINGTILLS"
                    "HEFANCIEDSHEHEARDTHERABBITJUSTUNDERTHEWINDOWSHESUD"
                    "DENLYSPREADOUTHERHANDANDMADEASNATCHINTHEAIRSHEDIDN"
                    "OTGETHOLDOFANYTHINGBUTSHEHEARDALITTLESHRIEKANDAFAL"
                    "LANDACRASHOFBROKENGLASSFROMWHICHSHECONCLUDEDTHATIT"
                    "WASJUSTPOSSIBLEITHADFALLENINTOACUCUMBERFRAMEORSOME"
                    "THINGOFTHATSORTNEXTCAMEANANGRYVOICETHERABBITSPATPA"
                    "TWHEREAREYOUANDTHENAVOICESHEHADNEVERHEARDBEFORESUR"
                    "ETHENIMHEREDIGGINGFORAPPLESYERHONORHERECOMEANDHELP"
                    "MEOUTOFTHISNOWTELLMEPATWHATSTHATINTHEWINDOWSUREITS"
                    "ANARMYERHONORWELLITSGOTNOBUSINESSTHEREATANYRATEGOA"
                    "NDTAKEITAWAYTHEREWASALONGSILENCEAFTERTHISANDALICEC"
                    "OULDONLYHEARWHISPERSNOWANDTHENANDATLASTSHESPREADOU"
                    "THERHANDAGAINANDMADEANOTHERSNATCHINTHEAIRTHISTIMET"
                    "HEREWERETWOLITTLESHRIEKSANDMORESOUNDSOFBROKENGLASS"
                    "IWONDERWHATTHEYLLDONEXTTHOUGHTALICEASFORPULLINGMEO"
                    "UTOFTHEWINDOWIONLYWISHTHEYCOULDSHEWAITEDFORSOMETIM"
                    "EWITHOUTHEARINGANYTHINGMOREATLASTCAMEARUMBLINGOFLI"
                    "TTLECARTWHEELSANDTHESOUNDOFAGOODMANYVOICESALLTALKI"
                    "NGTOGETHERSHEMADEOUTTHEWORDSWHERESTHEOTHERLADDERBI"
                    "LLSGOTTHEOTHERBILLHEREBILLWILLTHEROOFBEARWHOSTOGOD"
                    "OWNTHECHIMNEYNAYISHANTYOUDOITHEREBILLTHEMASTERSAYS"
                    "YOUVEGOTTOGODOWNTHECHIMNEYALICEDREWHERFOOTASFARDOW"
                    "NTHECHIMNEYASSHECOULDANDWAITEDTILLSHEHEARDALITTLEA"
                    "NIMALSCRATCHINGANDSCRAMBLINGABOUTINTHECHIMNEYCLOSE"
                    "ABOVEHERTHENSHEGAVEONESHARPKICKANDWAITEDTOSEEWHATW"
                    "OULDHAPPENNEXTTHEFIRSTTHINGSHEHEARDWASAGENERALCHOR"
                    "USOFTHEREGOESBILLTHENTHERABBITSVOICEALONECATCHHIMY"
                    "OUBYTHEHEDGETHENSILENCEANDTHENANOTHERCONFUSIONOFVO"
                    "ICESHOLDUPHISHEADBRANDYNOWDONTCHOKEHIMWHATHAPPENED"
                    "TOYOULASTCAMEALITTLEFEEBLESQUEAKINGVOICEWELLIHARDL"
                    "YKNOWNOMORETHANKYEIMBETTERNOWALLIKNOWISSOMETHINGCO"
                    "MESATMELIKEAJACKINTHEBOXANDUPIGOESLIKEASKYROCKETAF"
                    "TERAMINUTEORTWOOFSILENCETHEYBEGANMOVINGABOUTAGAINA"
                    "NDALICEHEARDTHERABBITSAYABARROWFULWILLDOTOBEGINWIT"
                    "HABARROWFULOFWHATTHOUGHTALICEBUTSHEHADNOTLONGTODOU"
                    "BTFORTHENEXTMOMENTASHOWEROFLITTLEPEBBLESCAMERATTLI"
                    "NGINATTHEWINDOWANDSOMEOFTHEMHITHERINTHEFACEALICENO"
                    "TICEDWITHSOMESURPRISETHATTHEPEBBLESWEREALLTURNINGI"
                    "NTOLITTLECAKESASTHEYLAYONTHEFLOORANDABRIGHTIDEACAM"
                    "EINTOHERHEADIFIEATONEOFTHESECAKESSHETHOUGHTITSSURE"
                    "TOMAKESOMECHANGEINMYSIZESOSHESWALLOWEDONEOFTHECAKE"
                    "SANDWASDELIGHTEDTOFINDTHATSHEBEGANSHRINKINGDIRECTL"
                    "YASSOONASSHEWASSMALLENOUGHTOGETTHROUGHTHEDOORSHERA"
                    "NOUTOFTHEHOUSEANDFOUNDQUITEACROWDOFLITTLEANIMALSAN"
                    "DBIRDSWAITINGOUTSIDETHEYALLMADEARUSHATALICETHEMOME"
                    "NTSHEAPPEAREDBUTSHERANOFFASHARDASSHECOULDANDSOONFO"
                    "UNDHERSELFSAFEINATHICKWOOD";

// Engima plain texts from WWII, Kriegsmarine, about U boats and stuff
// https://enigma.hoerenberg.com/index.php?cat=The%20U534%20messages
// P1030660-69,71-74, 76, 79, 81-88: 3722 chars
char testTextEnigma[]=
                    "FXDXUUUOSTYFUNCQUUUFXWTTXVVVUUUEINSEINSNULDREIKKEI"
                    "SELEKKXXISTSECHSSTUENDLICHESDOCKENVORMITTAGSAMDREI"
                    "XFUNFXINRENDSBURGGEMXFXDXUUUOSTMOEGLICHLUMSUEDDREI"
                    "YYVIEGINSCHAUERNBISFUENFWOLKIGVEREINZELTSCHAUERKKB"
                    "EREICHDORABISEINSXFUENFXABENDSKKSICHTACHTYYZEHNSMS"
                    "EEZWOYBDREIDBVUUUVIRSIBENNULEINSYNACHRXUUUSTUETZPU"
                    "NKTLUEBECKVVVCHEFVIERXUUUFLOTTXXMITUUUVIERSIBENNUL"
                    "ZWOUNDUUUVIERSIBENNULDREIZURFLENDERWERFTLUEBECKGEH"
                    "ENXFONDORTFOLGTWEITERESXUUUZWODREIDREIACHTVVVCHEFV"
                    "IERXUUUFLOTTXXMITUUUZWODREISECHSEINSZUMDOCKBETRIEB"
                    "RENDSBURGENTLASSENXMITUUUVIRSGBENNULZWOYVIRSIBENYU"
                    "LDREIYZWODREISECHSEINSUMEINSNULUHRJSASSNITZJAUSNAC"
                    "HISONDERBURGJFAHRTFUENFSMTNUUUEINSEINSNULDRFIKKEIS"
                    "ELEKKVODVONFDUUUOSTINGXXXSOFORTUMFANGDOVKARBEITHER"
                    "GEBENNMHAENGEMICTANGELEITUUUVDRSIBENNULEINSANXXUUU"
                    "ZWODREIDREIACHTKKKAISERKKSTUEKOWARNEMVENDEYZWOSECH"
                    "VXUUUFLOTTXHACHRXFXDXUUUAUSBILDUNGVVVHEVTHAXXEINSE"
                    "INSDGEINULUHRMITETWAEINEEINJNULNUOFLUECHTLIKGEMARS"
                    "BHWARNEMUENDENACHJNYBORGJJNYBORZNNANNIBRUNOBESETZT"
                    "KOMXADMXUUUBOOTEYFDUUUAUSBXYFDUUUOSTYSSSMMMHHHSSSO"
                    "STAZWWFUNFXUUUFLOTTXYFUNFXUUUFLOTTXYHAKAXKIELVONVO"
                    "NTORPXFANGBOOTEINSNEUNXXEINSZWOUHRJKIELWEINGELAUFE"
                    "NYFFFTTTBLEIBTBESETZTRSSDCHEFFUNFXUUUFLOTTXXEINSKK"
                    "TTTFFFEINSACHTMITZWOSTELLKARTENAUSRUESTUNGSKAGERRA"
                    "KUNDNORWEGENMITHAFENPLAENENUNDEINSEXEMPLARLFDXBEFX"
                    "BBBDDDUUUOOOPPPNRXSIEBENVOMZWOVIERXVIERXVIERFUNFXS"
                    "OFORTNEUSTADTINMARSCHSETZENXZWOKKTTTFFFEINBNEUNVOR"
                    "LPEUFIGKIELBLEIBENXDHEIKKBESTARTEHXNGERBETENXXFXDX"
                    "UUUAUSBXKFVONVONUUUEINSNULNULACHTKKGESSNERKRTXNULN"
                    "EUNDREINULUHRVONLUEBECKTRAVEMUENDEEINXZUSATZFUERFU"
                    "NFUUUFLOTTXJTREIBOALVERBRXVOMEINSXVIRXBISDREINULXV"
                    "IRXSECHSYEINSCBMXBESTANDAMEINSXFUNFXXVIRFUNFYNEUNC"
                    "BMFXDXUEUOSTKKINGXKKVONVONUUUEINSEINSNULDREIKKEISE"
                    "LEKKXXEINSKKUNDICHTIGKEITROHRVIERXKKBEYYUNDENTWAES"
                    "SERUNGKKZWOKKSCHWEISSENKUEHLWASXERLEITUNGINVIERBAC"
                    "KBORDLBQFUENFTEUUUFLOTTXANANUUUZWOFUEDFEINSSIEBENX"
                    "XSPEZIAHISTFUERTELEKINANLAGEWARTEAUFUUULEITSTELLEX"
                    "FRAGEXXWANNWIRDSPEZIALISTABGEHOLTXBNUUUJAGDKEINEVE"
                    "RAENDERUNGENGEGWNLEITNRXEINSVIERSIEBENXXADMXWWWEST"
                    "LXOSTSEELTNRXEINSFUENFZWOBBVEOMXADMXUUZBOOTEYFDUUU"
                    "AESBXYTORPXINSPEKTIONYUUUTENDERMOSELLONVONKREFELDX"
                    "XEINSEINSVIERNULKRKRALLEXXFOLGENDESISTSOFORTBEKANN"
                    "TZUGEBENXXICHHABEFOLGENDENBEFEHLERHALTENXXJANSTERL"
                    "EDESBISHERIGXNREICHSMARSCHALLSJGOERINGJSETZTDERFUE"
                    "HRERSIEYHVRRGRZSSADMIRALYALSSEINENNACHFOLGEREINXSC"
                    "HRIFTLSCHEVOLLMACHTUNTERWEGSXABSOFORTSOLLENSIESAEM"
                    "TLICHEMASSNAHMENVERFUEGENYDIESICHAUSDERGEGENWAERTI"
                    "GENLAGEERGEBENXGEZXREICHSLEITEIKKTULPEKKJBORMANNJX"
                    "XOBXDXMMMDURNHFKSTXKOMXADMXUUUBOOIEXKPFFFDDDUUUMST"
                    "VINVONHAKFROENNELQAUFGGGZWONULZWONEUXUUBZWOVOMZWOA"
                    "CHTXVSERXXJHRCHTJHIERNICHTEINZELGUFENFGRUPPENMARYM"
                    "HMITUUUDREINULZWOVIRUNBUUGZWOFUNFDREIOCHTNACHTRAVE"
                    "MUEVDEVVVUUUDREINULEINSVIRKKMARBACHKKMVVVUUUVIRSOB"
                    "ENNULEINSXXMITUUUVIRSIBENNULZWOYVIRSIBENNULDREIYZW"
                    "ODREISECHSEINSYZWODREIDREIACHTEINSDREIOITNACHWZSTE"
                    "NPASSIRTKOMXADMXUUUBOOTEYSSSMCMHHHSSSOSTHFXDXUFHOS"
                    "TVONVONCHEFVIERTEUUDFLOTTXSWGNEMUENDEANKERAUFXMARS"
                    "CHNACHHOERUPYYHAFFANGETRETENXSTELLENYNSWINEMUENDEH"
                    "ABENXZUSATZFUERLEITUNGXXGUSNAVFRITZUEBELSTELLTGEGE"
                    "NSTELLENVERKEHRABZWONULLNULLNULLUHREINBTSPRUCHSCHL"
                    "IESSELVONFFFTTTMILUHRZEITGRUPSEEINSZWOZHOVIERSOFOR"
                    "TWERGEBENYDAFFFTTTNICHTZULOESENFUNKLEITUNGXZNEISSE"
                    "FURKCRVXKPTXHEILMANNDVVFANGBTXZWOVIERXXXELABGABEDU"
                    "RCHGEFUHRTXKURIERMITSCALUEVSELMITCELNOCHNICHTEEUGE"
                    "TROFFENXFRGGESOLLIQHHIERWZRTWNOJSRWARNEMGNDQFFHENM"
                    "UUUYYSTUETZPUNKTWESERMUENDEYTTTFFFEINSFUNFYTTCFFFE"
                    "INSSIBENVONVONFXDXUUUOSTVVVOOOXXAUFDORTXFFFTTTEINS"
                    "FUNFVIERNEUNYYZWONEUNVOMZGONEUNXVIERXXEINSKKVONDEN"
                    "FUNFNZLTONMEQXERFUEGBARENHROVIJDTETWADREINULTONNEN"
                    "MOEGLICHSTVOLLSTAENDIGEUUUYYBOOTSAUSRUPSTUNPENZMSA"
                    "MMENSTELLENUNDMITTTTFFFEINSFUNFNNCHHQLGOLRNDVERLAG"
                    "ERNXZWOKKRESTPROVIANTVONETWAZWONULTONNENMITTTTFFFT"
                    "INSSIBENANJSAARJRENWSBURGABGEBENXZUSATZVUERTTTFFFE"
                    "INSFUNFUNDTTTFFFEINSSIBENXXSOFORTAUSLAUFENNACHWESE"
                    "RMUENDEXBESTAETIGENCWQ";

/**************************************************************************************************\
* 
* Test the Engima M3
* 
\**************************************************************************************************/
void testEnigmaM3(void)
{
    testStart("Enigma M3");
    // Test 1
    Enigma* enigma=createEnigmaM3();
    setText(enigma, "RCGXFEAJCT");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "II");
    placeWalze(enigma, 3, "III");
    
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 17);
    setRingStellung(enigma, 3, 12);

    setGrundStellung(enigma, 1, 'A');
    setGrundStellung(enigma, 2, 'B');
    setGrundStellung(enigma, 3, 'C');

    clearSteckerBrett(enigma);
    encodeDecode(enigma);

    char* result=toString(enigma);
    assertStringEquals("HEILHITLER", result);
    
    // Test 2
    setText(enigma, "mpyjllkozy");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 1);
    setRingStellung(enigma, 3, 1);

    setGrundStellung(enigma, 1, 'P');
    setGrundStellung(enigma, 2, 'D');
    setGrundStellung(enigma, 3, 'U');
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("HEILHITLER", result);


    // Test 3
    setText(enigma, "boot klar x bei j schnoor j etwa zwo siben x nov x sechs nul cbm x proviant bis zwo nul x dez x benoetige glaeser y noch vier klar x stehe marqu bruno bruno zwo funf x lage wie j schaefer j x nnn www funf y eins funf mb steigend y gute sicht vvv j rasch");

    placeWalze(enigma, 1, "VI");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "III");

    placeUmkehrWalze(enigma, "UKW C");
    
    setRingStellungen(enigma, "06 23 12");

    setGrundStellungen(enigma, "01 17 12");
    
    placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
    
    encodeDecode(enigma);

    result=toString(enigma);
    assertStringEquals("YYTHXJLXTEUXAJGMUWMGWAFQLUFTHQMFYEXMGCZDUQGPNNFEHRUUWNGMFMQADFLWZNQVVWQWGXHBGGNXHWFLCOUZPDZAPTUBQHEOQIMFTQJQDKDEFPPUBAGWZZXSCJYXPPWUPSTHNVKAMNACPYAPROEEZFZCTNCHQJUMIELXPUZFTHHYIPAOLYXWXOWQZJQNJLYI", result);
    
    destroyEnigma(enigma);  
    testWrapUp();
}

/**************************************************************************************************\
* 
* Long text (1000 char) so the left most Walze has been advanced at least 1 position
* 
\**************************************************************************************************/
char longTextExpected[]=
                    "RCGYXHBPLODPQOERWVDDYLAOTKTNZQEMUIUOKNJEBWOBISTBGU"
                    "YIYXOJZIHPRZFWWPAFWFSVJFYVDZJLQYXRJKKDYQVWXGSQUFOU"
                    "QQRHUOFYTJHGONBLBSNBHRELPQITJQKCPXUDSPLNYOBKVVBORY"
                    "XMDAKUWAZINQKONFNJNARLXGAJDHYCGXGQAGJXOEJVKQVCQPLK"
                    "PTCXYLBCNLSIKRLSHUKQKMCHDCCLADKPEGUILAGUXADAXIWOGG"
                    "PBMZFAUTWCMHLQQMEKJQPDXAYOEVYLUZPYTLCRRKYTSYWFMPKY"
                    "ASIFTFYTVVLTQBWEJUDQCNYGHRIULFESGAECZMLSTETKLLUIZL"
                    "ZYEUNBXLPPDAJECJNAPAUKAPKNKLVUCGNTVQTIGBQPZDTQSFKK"
                    "JKQONXHTVRWHRWBWLVLYKWEGBTIRUOINSROSIPRGMHANFEOFKY"
                    "ZIUXTJHYJBQWDYCEODEROHHSPNAESXNMQIOIORMRZCXWVQCGBC"
                    "XSSCGGBJDLVPVRDRBNMJDBQXJPMECOFEFDDRLFSWCYUWJREGPE"
                    "RCZCPTSYVHSVHFDINOYLBZQZUTRANPJWYXZMIVMQHPIVOIQYZM"
                    "SMTYQELLUFFPAUPUEYTNGEYKZDQUMJLDRIWZPNPUZBNVIWSZXC"
                    "NBTATWDDJDUCILPCUSGYKOCNFSMQLZSCCTBRQNEEPBSIETTJJP"
                    "XREOJZAALUSQGVAYGDQLOJNQOPLIBCDUSAZWQPWAQVAPECLCTZ"
                    "LLAPSAWNAZPDNZCFJYGDZCTJYPBZIBZBYXSPRRMEOYGZWRFDEB"
                    "CSWBYOYTCWSMDPPETWPXFYTCIIZFEQEUHEMVOGYGCSQWPGUFPO"
                    "CCAJKFPTKYBUZPNWMZCCXIEBFZEXXFYOWVIWSZHCHMIRLYNSPP"
                    "WBOSMIDNNMDKQFFAHZSMOLTYTIYZQLAMWKEYPHQGKJBRPBLEHL"
                    "MVTPMLINLGYQUIURZDZSMUQREGGTKKFOODEKAWTUAKVMNVFWSD";


void testEngimaM3Long()
{
    testStart("Enigma M3 Long");
    // Test 1
    Enigma* enigma=createEnigmaM3();
    setText(enigma, testTextRandom);
    placeWalze(enigma, 1, "VI");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "III");
    
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 3);
    setRingStellung(enigma, 3, 2);

    setGrundStellung(enigma, 1, 1);
    setGrundStellung(enigma, 2, 17);
    setGrundStellung(enigma, 3, 12);

    clearSteckerBrett(enigma);
    placeSteckers(enigma, "bq cr di ej kw mt os px uz gh");
    encodeDecode(enigma);

    char* result=toString(enigma);
    assertStringEquals(longTextExpected, result);

    testWrapUp();
}

/**************************************************************************************************\
* 
* Test the Engima M4
* 
\**************************************************************************************************/

char testM4Text[]=
"boot klar x bei j schnoor j etwa zwo siben x nov x sechs nul cbm x proviant bis zwo "
"nul x dez x benoetige glaeser y noch vier klar x stehe marqu bruno bruno zwo funf x "
"lage wie j schaefer j x nnn www funf y eins funf mb steigend y gute sicht vvv j rasch";

char testM4Cipher[]=
"SFNXFPQKKAISGWNNTBSTLMJOOWMFJXWIMCVRWALUSDDIIVQGXTEOAXZXURQUXPXDKZYNEMDZTDBEUPNKINOJH"
"YUGZTPLKHAAMIZHSIWXGPHPQPTZDNBCFQDWACPFTXJKWVZGIFVBJYIAWFNXIWDIOXLAZDOXHNAPZXPSSJQSTT"
"HKAIDMSCCNSBLYDLBOLRQNDZUO";

void testEnigmaM4(void)
{
    testStart("Enigma M4 Shark");
    // Test 1
    Enigma* enigma=createEnigmaM4();
    setText(enigma, "xfla kyyn ba");
    placeWalze(enigma, 1, "Beta");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "II");
    placeWalze(enigma, 4, "III");
    placeUmkehrWalze(enigma, "UKW B2");
    setRingStellung(enigma, 1, 1);
    setRingStellung(enigma, 2, 17);
    setRingStellung(enigma, 3, 12);
    setRingStellung(enigma, 4, 15);
    setGrundStellung(enigma, 1, 'A');
    setGrundStellung(enigma, 2, 'B');
    setGrundStellung(enigma, 3, 'C');
    setGrundStellung(enigma, 4, 'D');
    clearSteckerBrett(enigma);
    encodeDecode(enigma);
    char* result=toString(enigma);
    assertStringEquals("HEILHITLER", result);

    // Test 3
    setText(enigma, testM4Text);
    placeWalze(enigma, 1, "Beta");
    placeWalze(enigma, 2, "VIII");
    placeWalze(enigma, 3, "VI");
    placeWalze(enigma, 4, "II");
    placeUmkehrWalze(enigma, "UKW B2");
    setRingStellungen(enigma, "23 17 15 15");
    setGrundStellungen(enigma, "02 02 25 04");
    placeSteckers(enigma, "aj kf bc rt lm");
    encodeDecode(enigma);
    result=toString(enigma);
    assertStringEquals(testM4Cipher, result);

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Test The double step thing for Walze 2
* QPP -> QPQ -> QQR -> RRS -> RRT -> RRU -> ...
* 
\**************************************************************************************************/
void testEnigmaDoubleStep()
{
    testStart("Double step");
    Enigma* enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "I");
    placeWalze(enigma, 3, "I");
    
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'Q');
    setGrundStellung(enigma, 2, 'P');
    setGrundStellung(enigma, 3, 'P');

    clearSteckerBrett(enigma);
    encodeDecode(enigma);
    char* result=toString(enigma);
    assertStringEquals("ZGOUZFVQWG", result);
    testWrapUp();
}

/**************************************************************************************************\
*
* Test the advance/reverse function
* 
\**************************************************************************************************/
void testEnigmaAdvanceReverse()
{
    testStart("Advance/Reverse");
    Enigma* enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "I");
    placeWalze(enigma, 2, "III");
    placeWalze(enigma, 3, "II");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'Q');
    setGrundStellung(enigma, 2, 'U');
    setGrundStellung(enigma, 3, 'D');
    clearSteckerBrett(enigma);
    
    // Walze 3
    advances(enigma, 1);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('E', stellungToChar(getGrundStellung(enigma, 3)));
    
    // Walze 1, 2, 3
    advances(enigma, 1);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('V', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('F', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 2, 3 (double step of Walze 2)
    advances(enigma, 1);
    assertIntEquals('R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('G', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 3
    advances(enigma, 1);
    assertIntEquals('R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 3
    advances(enigma, 1);
    assertIntEquals('R', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('W', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('I', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 3 reverses
    advances(enigma, -1);
    assertIntEquals('R', stellungToChar(getGrundStellung(enigma, 1))); 
    assertIntEquals('W', stellungToChar(getGrundStellung(enigma, 2))); 
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 3 reverses
    advances(enigma, -1);
    assertIntEquals('R', stellungToChar(getGrundStellung(enigma, 1))); 
    assertIntEquals('W', stellungToChar(getGrundStellung(enigma, 2))); 
    assertIntEquals('G', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 2, 3 (double step) reverse
    advances(enigma, -1);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1))); // R
    assertIntEquals('V', stellungToChar(getGrundStellung(enigma, 2))); // W
    assertIntEquals('F', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 1, 2, 3 reverse
    advances(enigma, -1);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('E', stellungToChar(getGrundStellung(enigma, 3)));

    // Walze 3 reverse -> returned to start position
    advances(enigma, -1);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('D', stellungToChar(getGrundStellung(enigma, 3)));

    advances(enigma,  52348);
    advances(enigma, -52348);
    assertIntEquals('Q', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('U', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('D', stellungToChar(getGrundStellung(enigma, 3)));
    testWrapUp();
}

/**************************************************************************************************\
*
* Test the advance/reverse function 2
* 
\**************************************************************************************************/
void testEnigmaAdvanceReverse2()
{
    testStart("Advance/Reverse 2");
    Enigma* enigma=createEnigmaM3();
    setText(enigma, "AAAAAAAAAA");
    placeWalze(enigma, 1, "IV");
    placeWalze(enigma, 2, "V");
    placeWalze(enigma, 3, "II");
    placeUmkehrWalze(enigma, "UKW B");
    
    setRingStellung(enigma, 1, 'A');
    setRingStellung(enigma, 2, 'A');
    setRingStellung(enigma, 3, 'A');

    setGrundStellung(enigma, 1, 'I');
    setGrundStellung(enigma, 2, 'Y');
    setGrundStellung(enigma, 3, 'H');
    clearSteckerBrett(enigma);
    
    // 
    advances(enigma, 26);
    assertIntEquals('J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('A', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));
    
    // 
    advances(enigma, 26);
    assertIntEquals('J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('B', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));

    //
    advances(enigma, -26);
    assertIntEquals('J', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('A', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));

    //
    advances(enigma, -26);
    assertIntEquals('I', stellungToChar(getGrundStellung(enigma, 1)));
    assertIntEquals('Y', stellungToChar(getGrundStellung(enigma, 2)));
    assertIntEquals('H', stellungToChar(getGrundStellung(enigma, 3)));
    testWrapUp();
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void testEnigmaCounts()
{
    testStart("Counts");
    
    char trigram[3]="AZZ";
    char bigram[2]="ZZ";
    int  counts[MAX_POSITIONS];
    
    Enigma* enigma=createEnigmaM3();
    
    enigma->textSize=15;
    enigma->conversion[ 0]='A'-'A';
    enigma->conversion[ 1]='Z'-'A';
    enigma->conversion[ 2]='Z'-'A';
    enigma->conversion[ 3]='A'-'A';
    enigma->conversion[ 4]='B'-'A';
    enigma->conversion[ 5]='C'-'A';
    enigma->conversion[ 6]='A'-'A';
    enigma->conversion[ 7]='Z'-'A';
    enigma->conversion[ 8]='Z'-'A';
    enigma->conversion[ 9]='Z'-'A';
    enigma->conversion[10]='B'-'A';
    enigma->conversion[11]='P'-'A';
    enigma->conversion[12]='A'-'A';
    enigma->conversion[13]='Z'-'A';
    enigma->conversion[14]='Z'-'A';
    
    assertIntEquals(4, countLetter(enigma, 'A'));
    assertIntEquals(2, countLetter(enigma, 'B'));

    countConvertedChars(enigma, counts);
    assertIntEquals(4, counts[0]);
    assertIntEquals(2, counts[1]);

    assertIntEquals(3, countTrigram(enigma, trigram));
    assertIntEquals(4, countNgram(enigma, bigram, 2));

    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Test creation of random settings
* 
\**************************************************************************************************/
void testEnigmaRandomSettings()
{
    testStart("Random settings");
    srand(1);
    Enigma* enigma;
    EnigmaSettings* settings;
    enigma=createEnigmaM3();
    settings=createRandomSettings(enigma, M3_ARMY_1938, 5);
    // Might fail on other machines
    // TO DO: solve
    assertIntEquals(3, settings->numberOfWalzen);
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);

    enigma=createEnigmaM4();
    settings=createRandomSettings(enigma, M4_NAVAL_1941, 5);
    // Might fail on other machines
    // TO DO: solve
    assertIntEquals(4, settings->numberOfWalzen);
    destroyEnigmaSettings(settings);
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Enigma with random settings, encode, decode
* 
\**************************************************************************************************/
void testEnigmaStress()
{
    testStart("Stress test");
    Enigma*         enigma;
    EnigmaSettings* settings;
    char*           test    ="TESTSTRING";
    char*           result;
    int             i;
    enigma=createEnigmaM3();
    for (i=0;i<1000;i++)
    {
        settings=createRandomSettings(enigma, M3_ARMY_1938, 5);

        strncpy(settings->cipher, test, MAX_TEXT-1);
        setEnigma(enigma, settings);
        encodeDecode(enigma);
        result=toString(enigma);

        strncpy(settings->cipher, result, MAX_TEXT-1);
        setEnigma(enigma, settings);
        encodeDecode(enigma);
        result=toString(enigma);

        assertStringEquals(test, result);

        destroyEnigmaSettings(settings);
    }
    destroyEnigma(enigma);
    testWrapUp();
}

/**************************************************************************************************\
*
* Enigma test Walze/UKW permutations
* 
\**************************************************************************************************/
void testEnigmaGenerateWalzenPermutations()
{
    testStart("Walze permutations");

    LinkedList* perms;
    int*        p;
    perms=generateWalzePermutations(ENIGMATYPE_M3, M3_ARMY_1938);
    assertIntEquals(120, linkedListLength(perms));
    p=(int*)elementAt(perms, 0);
    assertStringEquals("UKW B", umkehrWalzeNames[p[0]]);
    assertStringEquals("I"    , walzeNames[p[1]]);
    assertStringEquals("II"   , walzeNames[p[2]]);
    assertStringEquals("III"  , walzeNames[p[3]]);
    p=(int*)elementAt(perms, 60);
    assertStringEquals("UKW C", umkehrWalzeNames[p[0]]);
    assertStringEquals("I"    , walzeNames[p[1]]);
    assertStringEquals("II"   , walzeNames[p[2]]);
    assertStringEquals("III"  , walzeNames[p[3]]);
    p=(int*)elementAt(perms, 119);
    assertStringEquals("UKW C", umkehrWalzeNames[p[0]]);
    assertStringEquals("V"    , walzeNames[p[1]]);
    assertStringEquals("I"    , walzeNames[p[2]]);
    assertStringEquals("II"   , walzeNames[p[3]]);
    destroyPermutations(perms);

    perms=generateWalzePermutations(ENIGMATYPE_M4, M4_NAVAL_1941);
    assertIntEquals(1344, linkedListLength(perms));
    p=(int*)elementAt(perms, 0);
    assertStringEquals("UKW B2", umkehrWalzeNames[p[0]]);
    assertStringEquals("Beta"  , walzeNames[p[1]]);
    assertStringEquals("I"     , walzeNames[p[2]]);
    assertStringEquals("II"    , walzeNames[p[3]]);
    assertStringEquals("III"   , walzeNames[p[4]]);
    p=(int*)elementAt(perms, 1343);
    assertStringEquals("UKW C2", umkehrWalzeNames[p[0]]);
    assertStringEquals("Gamma" , walzeNames[p[1]]);
    assertStringEquals("VIII"  , walzeNames[p[2]]);
    assertStringEquals("I"     , walzeNames[p[3]]);
    assertStringEquals("II"    , walzeNames[p[4]]);
    destroyPermutations(perms);

    testWrapUp();   
}


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testEnigma()
{
    moduleTestStart("Enigma");
    testEnigmaM3();
    testEngimaM3Long();
    testEnigmaM4();
    testEnigmaDoubleStep();
    testEnigmaAdvanceReverse();
    testEnigmaAdvanceReverse2();
    testEnigmaCounts();
    testEnigmaRandomSettings();
    testEnigmaStress();
    testEnigmaGenerateWalzenPermutations();
    moduleTestWrapUp();
}