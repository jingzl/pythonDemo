//
//  ZNAppConstants.h
//  ZDCNavi
//
//  Created by Satoshi Muraki on 08/09/15.
//  Copyright 2008 SONORAN BLUE. All rights reserved.
//

#ifndef __ZN_APP_CONSTANTS_H__
#define __ZN_APP_CONSTANTS_H__

/*
    @abstract   URL起動で利用される文字列
    @define     kZNAppScheme
                    ライト版/フル版共通のスキーム
    @define     kZNAppCommandSetCard
                    指定された場所にカードを置き、住所を逆引きするコマンド名
    @define     kZNAppCommandSearchWord
                    フリーワード検索を行うコマンド名
    @define     kZNAppCommandSearchRoute
                    ルート探索を行うコマンド名
    @define     kZNAppCommandDebug
                    デバッグ画面の表示コマンド名
*/
#define kZNAppScheme                        "zdcnavi"
#define kZNAppCommandSetCard                "setcard"
#define kZNAppCommandSearchWord             "searchword"
#define kZNAppCommandSearchRoute            "searchroute"
#define kZNAppCommandDebug                  "debug"

/*
    @abstract   ブックマークのコピーで利用される文字列の定義
    @define     kZNAppLiteScheme                    @"zdcnavilite"      // v2
                    ライト版のスキーム
    @define     kZNAppLiteCommandExportBookmarks    @"exportbookmarks"  // v2
                    ブックマークの書き出しを依頼するコマンド名
    @define     kZNAppFullScheme                    @"zdcnavifull"      // v2
                    フル版のスキーム
    @define     kZNAppFullCommandImportBookmarks    @"importbookmarks"  // v2
                    ブックマークの読み込みを依頼するコマンド名
*/
#define kZNAppLiteScheme                    "zdcnavilite"      // v2
#define kZNAppLiteCommandExportBookmarks    "exportbookmarks"  // v2

#define kZNAppFullScheme                    "zdcnavifull"      // v2
#define kZNAppFullCommandImportBookmarks    "importbookmarks"  // v2

/*
    @abstract   URL起動で利用されるパラメータ名
    @define     kZNAppCommandParamLongitude
                    経度
    @define     kZNAppCommandParamLatitude
                    緯度
    @define     kZNAppCommandParamName
                    名前
    @define     kZNAppCommandParamWord
                    検索ワード
    @define     kZNAppCommandParamStartLongitude
                    出発地の経度
    @define     kZNAppCommandParamStartLatitude
                    出発地の緯度
    @define     kZNAppCommandParamStartName
                    出発地の名前
    @define     kZNAppCommandParamEndLongitude
                    目的地の経度
    @define     kZNAppCommandParamEndLatitude
                    目的地の緯度
    @define     kZNAppCommandParamEndName
                    目的地の名前
    @define     kZNAppCommandParamPassLongitude
                    経由地の経度
    @define     kZNAppCommandParamPassLatitude
                    経由地に緯度
    @define     kZNAppCommandParamPassName
                    経由地の名前
    @define     kZNAppCommandParamCarType
                    利用車種
    @define     kZNAppCommandParamUseHighwayFlag
                    有料道路を利用
    @define     kZNAppCommandParamHighwayFlag
                    高速道路上から出発
    @define     kZNAppCommandParamTimeFlag
                    距離優先 (0:距離優先 1:時間優先)
    @define     kZNAppCommandParamStraightFlag
                    直進優先
    @define     kZNAppCommandParamData              @"data" // v2
                    ブックマークのデータをやり取りするためのパラメータ
*/
#define kZNAppCommandParamLongitude         "lon"
#define kZNAppCommandParamLatitude          "lat"
#define kZNAppCommandParamName              "name"
#define kZNAppCommandParamWord              "frwd"
#define kZNAppCommandParamStartLongitude    "stx"
#define kZNAppCommandParamStartLatitude     "sty"
#define kZNAppCommandParamStartName         "stn"
#define kZNAppCommandParamEndLongitude      "edx"
#define kZNAppCommandParamEndLatitude       "edy"
#define kZNAppCommandParamEndName           "edn"
#define kZNAppCommandParamPassLongitude     "mx"
#define kZNAppCommandParamPassLatitude      "my"
#define kZNAppCommandParamPassName          "mn"
#define kZNAppCommandParamCarType           "rt"
#define kZNAppCommandParamUseHighwayFlag    "usehigh"
#define kZNAppCommandParamHighwayFlag       "high"
#define kZNAppCommandParamTimeFlag          "dtf"
#define kZNAppCommandParamStraightFlag      "str"
#define kZNAppCommandParamData              "data" // v2

/*
    @abstract   操作音関連の定数
    @define     kZNAppClickSoundDirectoryName
                    アプリバンドル内の操作音のフォルダ名
    @define     kZNAppClickSoundExtension
                    サウンドファイルの拡張子 ( CoreAudioFile )
    @define     kZNAppClickSoundDefaultName
                    デフォルトの操作音名
*/
#define kZNAppClickSoundDirectoryName       "ClickSounds"
#define kZNAppClickSoundExtension           "caf"
#define kZNAppClickSoundDefaultName         "Poi Low"


/*
    @abstract   GPS取得フィルター関連の定数
    
    @define     kZNAppLocationStartAccuracy1        160.0
                    開始から kZNAppLocationStartTime1 までの時間は
                    始めのポイントを取得するまで、この値でフィルタリングする
    @define     kZNAppLocationStartAccuracy2        400.0
                    開始から kZNAppLocationStartTime2 までの時間は
                    始めのポイントを取得するまで、この値でフィルタリングする
    @define     kZNAppLocationStartAccuracy3        1000.0
                    kZNAppLocationStartTime2 以降の時間は
                    始めのポイントを取得するまで、この値でフィルタリングする
    @define     kZNAppLocationStartTime1            5.0
                    kZNAppLocationStartAccuracy1 が有効な時間
    @define     kZNAppLocationStartTime2            10.0
                    kZNAppLocationStartAccuracy2 が有効な時間
    
    @define     kZNAppLocationGeneralAccuracy       160.0
                    2点目以降は、この精度より低いポイントは無効とする
                    0 の場合にはフィルタリングしない
    
    @define     kZNAppLocationRightAccuracy         20.0
                    正確なポイントと判断するための精度のしきい値
    @define     kZNAppLocationRightLocationCount    5
                    正確なポイントをこのポイント数取得したときにフィルターがオンになる
    
    @define     kZNAppLocationFilterSpeed          300.0   // (km / h)
                    GPSのポイントがこのスピード以上で移動した場合、
                    無効なポイントと判断される
                
    @define     kZNAppLocationUpdateInterval        3.0
                    同一ポイントでもこの時間（秒）たった場合は新しいポイントとして処理する
    
    @define     kZNAppLocationValueFactor           0.9 // v2
                    ヘッドアップを計算するときの古いベクトルへの係数
*/
#define kZNAppLocationStartAccuracy1        160.0
#define kZNAppLocationStartAccuracy2        400.0
#define kZNAppLocationStartAccuracy3        1000.0
#define kZNAppLocationStartTime1            5.0
#define kZNAppLocationStartTime2            10.0

#define kZNAppLocationGeneralAccuracy       160.0
#define kZNAppLocationRightAccuracy         20.0
#define kZNAppLocationRightLocationCount    5
#define kZNAppLocationFilterSpeed           300.0   // (km / h)
#define kZNAppLocationUpdateInterval        3.0
#define kZNAppLocationValueFactor           0.9 // v2 

/*
    @abstract   リルートの閾値に関連する定数
    @define     kZNAppScopeNormalMin        一般道のリルート閾値の最小値
    @define     kZNAppScopeNormalMax        一般道のリルート閾値の最大値
    @define     kZNAppScopeHighwayMin       高速道のリルート閾値の最小値
    @define     kZNAppScopeHighwayMax       高速道のリルート閾値の最大値
    @define     kZNAppHighwayLinkThreshold  高速道路から引き付ける距離の最大値
*/

#define kZNAppScopeNormalMin                10
#define kZNAppScopeNormalMax                33
#define kZNAppScopeHighwayMin               66
#define kZNAppScopeHighwayMax               300
#define kZNAppHighwayLinkThreshold          800.0
//added by wang.ss at 2010-06-06
#define kZNAppScopeAllMode					33
//end by wang.ss
/*
    @abstract   地図描画の初期値
    @define     kZNAppMapDefaultLevel       kZNMapLevel_i
    @define     kZNAppMapDefaultLevelScale  1.0
*/
#define     kZNAppMapDefaultLevel           kZNMapLevel_i
#define     kZNAppMapDefaultLevelScale      1.0

/*   
    @abstract   地図の有効範囲（ミリ秒）		台灣地圖有效範圍(千分之一秒)
    @define     kZNAppMapLongitudeMin       374400000
    @define     kZNAppMapLongitudeMax       604800000
    @define     kZNAppMapLatitudeMin        43200000
    @define     kZNAppMapLatitudeMax        198000000
*/

//-> modify by huangjj at 2010/04/07
/*
#define kZNAppMapLongitudeMin               374400000
#define kZNAppMapLongitudeMax               604800000
#define kZNAppMapLatitudeMin                43200000
#define kZNAppMapLatitudeMax                198000000
*/
//Modify by libin for PalmIphone <<<<<<<<<<  TODO: Need modify AppScale to China
#define kZNAppMapLongitudeMin               0	//modify by Ivanka 20101223 
#define kZNAppMapLongitudeMax               44280000000
#define kZNAppMapLatitudeMin                0
#define kZNAppMapLatitudeMax                9540000000	//edit Yakumo 20110124

#define palm_navi_longitudeMin              415486695.6 //add liutch  北京经纬度范围
#define palm_navi_longitudeMax              423048528
#define palm_navi_latitudeMin               141983085.6
#define palm_navi_latitudeMax               147818138.4
//Modify by libin for PalmIphone >>>>>>>>>>
/*
#define kZNAppMapLongitudeMin               424800000	//modify by Ivanka 20101223 
#define kZNAppMapLongitudeMax               442800000
#define kZNAppMapLatitudeMin                76800000
//#define kZNAppMapLatitudeMax                93600000
#define kZNAppMapLatitudeMax                95400000	//edit Yakumo 20110124
*/
//<- modify by huangjj at 2010/04/07

/*
    @abstract   地図レイヤの定数
    @define     kZNAppMapLayerDaytimeName           @"TaiwanDay"
                    "昼"用のレイヤ設定ファイル名
    @define     kZNAppMapLayerNighttimeName         @"TaiwanNight"
                    "夜"用のレイヤ設定ファイル名
    @define     kZNAppMapLayerUpdateInterval        60 // (60 sec = 1 min)
                    レイヤの更新のチェックを行う間隔
*/
#define kZNAppMapLayerDaytimeName           @"TaiwanDay"
#define kZNAppMapLayerNighttimeName         @"TaiwanNight"
#define kZNAppMapLayerUpdateInterval        60 // (60 sec = 1 min)

/*
    @abstract   地図タイルのデータ取得を行うタイミングに関する定数
    @define     kZNAppMapUpdateSubstitionTilesInterval
                    通常 1.0 秒置きに代替レイヤーのタイル計算を行う
*/
#define kZNAppMapUpdateSubstitionTilesInterval  1.0

/*
    @abstract   ルート探索の定数
    @define     kZNAppRoutePassPointMax             3
                    ルート探索できる経由地の最大値
    @define     kZNAppRouteGetAddressTimeOut        5.0
                    住所逆引きのタイムアウト時間（秒）
    @define     kZNAppRouteLocationTimeOut          60.0
                    現在地取得のタイムアウト時間（秒）
    @define     kZNAppRouteMatchLocationDelay       0.25
                    GPS の位置をライブラリに渡してから、ライブラリの routeMatchLocation:
                    関数を呼ぶまでの遅延時間。ライブラリではルートマッチの処理を同期的に
                    行っているため 250msec 遅らせなければいけない
*/
#define kZNAppRoutePassPointMax             3
#define kZNAppRouteGetAddressTimeOut        5.0
#define kZNAppRouteLocationTimeOut          60.0
#define kZNAppRouteMatchLocationDelay       0.25


/*
    @abstract   ルート履歴の定数
    @define     kZNAppRouteHistoryMax               10
                    ルート履歴の最大数
    @define     kZNAppRouteHistoryFileName          @"RouteHistory.plist"
                    ルート履歴のファイル名
    @define     kZNAppRouteFileName                 @"Route.plist"
                    ルート情報（最後に探索したルート、表示用のルート、通過した経由地情報など）
                    を保存するプロパティリストのファイル名
*/
#define kZNAppRouteHistoryMax               20
#define kZNAppRouteHistoryFileName          @"RouteHistory.plist"
#define kZNAppRouteFileName                 @"Route.plist"


/*
    @abstract   拠点情報画面のアイコンに関する定数
    @define     kZNAppSpotImageWidth                64.0
                    アイコンの幅
    @define     kZNAppSpotImageHeight               64.0
                    アイコンの高さ
    @define     kZNAppSpotImageCardOffsetX          5.0
                    アイコンの原点からカードの原点までのX軸の距離
    @define     kZNAppSpotImageCardOffsetY          8.0
                    アイコンの原点からカードの原点までのY軸の距離
*/
#define kZNAppSpotImageWidth                100.0	//64.0				// wangyang 10-05-12
#define kZNAppSpotImageHeight               120.0	//64.0				// wangyang 10-05-12
#define kZNAppSpotImageCardOffsetX          23.0	//5.0	(100-54)/2	// wangyang 10-05-12
#define kZNAppSpotImageCardOffsetY          35.0	//8.0	(120-49)/2	// wangyang 10-05-12


/*
    @abstract   タイマー処理の待ち時間
    @define     kZNAppApproachAlertInterval         60.0
                    "目的地付近です" のアラートを表示するまでの待ち時間
    @define     kZNAppAutoRerouteInterval           60.0
                    "ルートを外れました" のアラートを表示してから
                    次のアラートを自動的に表示するまでの時間
    @define     kZNAppAutoLockInterval              10.0
                    ナビ中に自動的に現在地ロックに移行するまでの時間
*/
#define kZNAppApproachAlertInterval         60.0
#define kZNAppAutoRerouteInterval           60.0
#define kZNAppAutoLockInterval              10.0 //10.0   //導航時自動鎖定之時間 （me箭頭歸位）
#define kZNAppGuideTableLockInterval		15.0	//add by huangjj at 2010/09/25 for bugNO.235

/*
    @abstract   拠点情報の履歴保存
    @define     kZNAppSpotRecentMax                 100
                    拠点履歴の最大数
    @define     kZNAppSpotAlwaysShownBookmarksMax   25
                    ブックマーク登録した拠点に「常に表示」を設定できる最大数
*/
#define kZNAppSpotRecentMax                 100
#define kZNAppSpotAddressBookMax            100 //li-zhj 10-4-2
#define kZNAppSpotMyHomeMax					1	// add by huangjj at 2010/04/07
#define kZNAppSpotAlwaysShownBookmarksMax   100	// modify by huangjj at 2010/04/10
#define kZNAppSpotStationRecordMax			20	// add bu huangjj at 2010/04/12
#define kZNAppOverSpeedRecordMax			3000//li-zhj 10-5-27

/*
    @abstract   拠点検索関連の定数
    @define     kZNAppSearchCacheDatabaseName       @"Search.db"
                    検索結果キャッシュのデータベースのファイル名
    @define     kZNAppSearchFavoriteMax             50
                    「よく見るジャンル」の最大数
    @define     kZNAppSearchLocationMax             50
                    「よく見る地域」の最大数
*/
#define kZNAppSearchCacheDatabaseName       @"Search.db"
#define kZNAppSearchFavoriteMax             50
#define kZNAppSearchLocationMax             50


/*
    @abstract   アドレス関連の定数
    @define     kZNAppAddressBookLabelKey           @"label"
                    アドレスのセルを表示するさいのキー
    @define     kZNAppAddressBookValueKey           @"value"
                    アドレスのセルを表示するさいのキー
*/
#define kZNAppAddressBookLabelKey           "label"
#define kZNAppAddressBookValueKey           "value"


/*
    @abstract   テーブルセルの識別子
    @define     kZNAppBookmarkTableCell     @"BookmarkTableCell"
                    ブックマーク画面のテーブルセル
*/
#define kZNAppDefaultTableCell              "Default"
#define kZNAppBookmarkTableCell             "BookmarkTableCell"

/*
    @abstract   走行規制に関する定数
    @define     kZNAppMovingSpeed           10.0
                    この速度（km/h）以上で移動している場合、走行規制を行う
    @define     kZNAppMovingUpdateInterval  1.5
                    この時間間隔（秒）で速度の計算を行う
 */

#define kZNAppMovingSpeed                   10.0
#define kZNAppMovingUpdateInterval          1.5

/*
    @abstract   経由地の反応エリアの定数
 */
#define kZNAppManueverTouchableWidth        30.0
#define kZNAppManueverTouchableHeight       30.0

// wangyang 10-05-31
#define	kZNAppVersionCheckResult_NoNewVersion		0
#define	kZNAppVersionCheckResult_NeedUpdateNow		1
#define	kZNAppVersionCheckResult_CanUpdateLater		2

/*
 @运行环境检查结果
 */
// wangyang 10-05-31
#define	kZNAppEnvCheckResult_OK				0
#define	kZNAppEnvCheckResult_CodeError		1
#define	kZNAppEnvCheckResult_PhoneTypeError	2
#define	kZNAppEnvCheckResult_OtherError		9

#endif /* __ZN_APP_CONSTANTS_H__ */
