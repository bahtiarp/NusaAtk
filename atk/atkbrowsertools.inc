<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage utils
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5306 $
   * $Id: atkbrowsertools.inc 5306 2007-09-28 16:01:24Z ivo $
   */

  /**
   * Utility for browser detection.
   *
   * @author Wim Kosten <wim@ibuildings.nl>
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage utils
   * @todo Move this class to the utils subpackage dir.
   */
  class browserInfo
  {
    var $ua           = "";
    var $full_version = "";
    var $browser      = "unknown";
    var $major        = 0;
    var $minor        = 0;
    var $os           = "";
    var $platform     = "";
    var $short        = "";
    var $brName       = "";
    var $osName       = "";
    var $hasGui       = 0;
    var $spider       = 0;
    var $family       = "";
    var $gecko        = 0;

   // constructor function

    function __construct($ua="")
    {
      $this->browserInfo($ua);
    }

    function browserInfo($ua="")
    {

     // set ua
      if (trim($ua) == "")
      {
       $this->ua = $_SERVER["HTTP_USER_AGENT"];
      }
      else
      {
       $this->ua = $ua;
      }

     // check the browser and set properties
      $this->detectAgent();

     // detect platform and OS
      $this->detectOS();

     // set the family
     if (preg_match("/gecko/i", $this->ua))
     {
      $this->gecko = 1;
     }

     switch (strtolower($this->browser))
     {
      case "msie":
       if ($this->major < 4)
       {
        $this->family = "ie3";
       }
       else if ($this->major >= 5)
       {
         $this->family = "ie5up";
       }
       else
       {
        $this->family = "ie4up";
       }
       break;

      case "opera":
       $this->family = "opera";
       break;

      case "netscape":
       if ($this->major < 6)
       {
        $this->family = "nn4";
       }
       else
       {
        $this->family = "nn6";
       }
       break;

      default:
       if ($this->gecko == 1)
       {
        $this->family = "gecko";
       }
       else
       {
        $this->family = "unknown";
       }
     }

     $this->short = $this->browser." ".$this->full_version;
    }

   // actual detection
    function detectAgent()
    {
      //MSIE
      $info=array();
      if (preg_match( 'MSIE ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) ||  preg_match( 'Microsoft Internet Explorer ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'MSIE';
       $this->brName       = "MS Internet Explorer";
       $this->hasGui       = 1;

       // check for Opera faking MSIE
        if (preg_match( 'Opera ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) || preg_match( 'Opera/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
        {
         $this->full_version = $info[1];
         $this->browser      = 'Opera';
         $this->brName       = "Opera";
         $this->hasGui       = 1;
        }

       // check for WebTV
        if (preg_match( 'WebTV/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
        {
         $this->full_version = $info[1];
         $this->browser      = 'WebTV';
         $this->brName       = "WebTV";
         $this->hasGui       = 1;
        }

       // check for AOL
        if (preg_match( 'AOL ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
        {
         $ver                = $this->full_version;
         $this->full_version = $info[1];
         $this->browser      = 'AOL (MSIE '.$ver.')';
         $this->brName       = "AOL";
         $this->hasGui       = 1;
        }
      }

     // Opera
      if (preg_match( 'Opera ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) || preg_match( 'Opera/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Opera';
       $this->brName       = "Opera";
       $this->hasGui       = 1;
      }

     // iCab
      elseif (preg_match( 'iCab ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) || preg_match( 'iCab/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'iCab';
       $this->brName       = "iCAB";
       $this->hasGui       = 1;
      }

     // Lynx
      elseif (preg_match( 'Lynx ([0-9].[0-9a-zA-Z.]{1,9})',$this->ua,$info) || preg_match( 'Lynx/([0-9].[0-9a-zA-Z.]{1,9})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Lynx';
       $this->platform     = 'Unix';
       $this->brName       = "Lynx (textbrowser)";
       $this->hasGui       = 0;
      }

     // Wget
      elseif (preg_match( 'Wget/([0-9].[0-9a-zA-Z.]{1,9})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Wget';
       $this->platform     = 'Unix';
       $this->brName       = "Wget";
       $this->hasGui       = 0;
      }

     // Snoopy (PHP HTTP stuff)
      elseif (preg_match( 'Snoopy v([0-9].[0-9a-zA-Z.]{1,9})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Snoopy';
       $this->brName       = "Snoopy";
       $this->hasGui       = 0;
      }

     // GetRight
      elseif (preg_match( 'GetRight/([0-9].[0-9a-zA-Z.]{1,9})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'GetRight';
       $this->platform     = 'Win9xNT';
       $this->brName       = "GetRight";
       $this->hasGui       = 0;
      }

     // KDE Konqueror
      elseif (preg_match( 'Konqueror ([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) || preg_match( 'Konqueror/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Konqueror';
       $this->platform     = 'Unix';
       $this->brName       = "KDE Konqueror";
       $this->hasGui       = 1;
      }

     // Netscape 6
      elseif (preg_match( 'Netscape6/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Netscape';
       $this->brName       = "Netscape";
       $this->hasGui       = 1;
      }

     // NCSA Mosaic
      elseif (preg_match( 'nsca mosaic ([0-9].[0-9a-zA-Z]{1,4})',strtolower($this->ua),$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'NSCA Mosaic';
       $this->brName       = "NSCA Mosaic";
       $this->hasGui       = 1;
      }

     // Prodigy Classic
      elseif (preg_match( 'prodigy-wb/([0-9].[0-9a-zA-Z]{1,4})',strtolower($this->ua),$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Prodigy Classic';
       $this->brName       = $this->browser;
       $this->hasGui       = 1;
      }

     // Nokia Communicator
      elseif (preg_match( 'Nokia-Communicator-WWW-Browser ([0-9].[0-9a-zA-Z]{1,4})',strtolower($this->ua),$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Nokia Communicator';
       $this->brName       = $this->browser;
       $this->hasGui       = 0;
      }

     //
     // CRAWLERS, SPIDERS ETC.
     //

     // Inktomi
      elseif (preg_match( 'Slurp/',$this->ua,$info))
      {
       $this->full_version = 0;
       $this->browser      = 'Inktomi Web Robot';
       $this->brName       = "Inktomi Web Robot";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Google
      elseif (preg_match( 'Googlebot/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Google WebCrawler';
       $this->brName       = "Google WebCrawler";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // ASPSeek
      elseif (preg_match( 'ASPSeek/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'ASPSeek';
       $this->brName       = "ASPSeek";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // ArchitextSpider
      elseif (preg_match( 'ArchitextSpider',$this->ua,$info))
      {
       $this->full_version = 0;
       $this->browser      = 'ArchitextSpider';
       $this->brName       = "ArchitextSpider";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Lycos spider
      elseif (preg_match( 'Lycos_Spider',$this->ua,$info))
      {
       $this->full_version = 0;
       $this->browser      = 'Lycos Spider';
       $this->brName       = "Lycos Spider";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Offline Explorer
      elseif (preg_match( 'Offline Explorer/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Offline Explorer';
       $this->brName       = "Offline Explorer";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Openbot/3.0
      elseif (preg_match( 'Openbot/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Openfind Data Gatherer';
       $this->brName       = "Openfind Data Gatherer";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Scooter
      elseif (preg_match( 'Scooter-([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info) || preg_match( 'Scooter-',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Scooter';
       $this->brName       = "Scooter";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // Sqworm/2.9.70-BETA
      elseif (preg_match( 'Sqworm/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'Sqworm';
       $this->brName       = "Sqworm";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // True_Robot/1.0
      elseif (preg_match( 'True_Robot/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'True_Robot';
       $this->brName       = "True Robot";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // libwww-perl/5.52
      elseif (preg_match( 'libwww-perl/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'libwww-perl';
       $this->brName       = "libwww-perl";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // psbot/0.1
      elseif (preg_match( 'psbot/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
      {
       $this->full_version = $info[1];
       $this->browser      = 'picsearch crawler';
       $this->brName       = "PicSearch Crawler";
       $this->hasGui       = 0;
       $this->spider       = 1;
      }

     // if we still haven't detected something
     // it might be just an old Netscape (pre Gecko)
      if ($this->browser == "unknown")
      {
       // if contains Mozilla then it is Netscape or just the Mozilla browser
       if (preg_match( 'Mozilla/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$info))
       {
        // if it also contains Gecko/ then it's a new Mozilla
        $dummy=array();
        if (preg_match( 'Gecko/([0-9].[0-9a-zA-Z]{1,4})',$this->ua,$dummy))
        {
         $info_1=array();
         preg_match('rv:([0-9].[0-9a-zA-Z].[0-9]{1,4})', $this->ua, $info_1);
         $this->full_version = $info_1[1];
         $this->browser      = 'Mozilla';
         $this->brName       = "Mozilla";
         $this->hasGui       = 1;
        }
        else
        {
         $this->full_version = $info[1];
         $this->browser      = 'Netscape';
         $this->brName       = "Netscape";
         $this->hasGui       = 1;
        }
       }
      }

     // now extract version
      if($this->full_version > 0)
      {
       $pos = strpos($this->full_version,".");
       if ($pos > 0)
       {
        $this->major = substr($this->full_version, 0, $pos);
        $this->minor = substr($this->full_version, $pos+1, strlen($this->full_version));
       }
       else $this->major = $this->full_version;
      }
    }

    /**
     * Detect wether the user is browsing with a PDA
     * Returns true if he is, an array if he is using Windows CE
     * and an array with ["width"] and ["height"] if those variables are known
     * @return mixed wether or not the user is using a PDA and size of the screen if it is passed along
     */
    public static function detectPDA()
    {
      $browser=substr(trim($_SERVER["HTTP_USER_AGENT"]),0,4);
      if ($browser=="Noki" || // Nokia phones and emulators
          $browser=="Eric" || // Ericsson WAP phones and emulators
          $browser=="WapI" || // Ericsson WapIDE 2.0
          $browser=="MC21" || // Ericsson MC218
          $browser=="AUR " || // Ericsson R320
          $browser=="R380" || // Ericsson R380
          $browser=="UP.B" || // UP.Browser
          $browser=="WinW" || // WinWAP browser
          $browser=="UPG1" || // UP.SDK 4.0
          $browser=="upsi" || // another kind of UP.Browser ??
          $browser=="QWAP" || // unknown QWAPPER browser
          $browser=="Jigs" || // unknown JigSaw browser
          $browser=="Java" || // unknown Java based browser
          $browser=="Alca" || // unknown Alcatel-BE3 browser (UP based?)
          $browser=="MITS" || // unknown Mitsubishi browser
          $browser=="MOT-" || // unknown browser (UP based?)
          $browser=="My S" || // unknown Ericsson devkit browser ?
          $browser=="WAPJ" || // Virtual WAPJAG www.wapjag.de
          $browser=="fetc" || // fetchpage.cgi Perl script from www.wapcab.de
          $browser=="ALAV" || // yet another unknown UP based browser ?
          $browser=="Wapa") // another unknown browser ("Wapalyzer"?)
        return true;
      else if (strstr($_SERVER["HTTP_USER_AGENT"],"Blazer")) // Palm's browser
      {
        return true;
      }
      else if (strstr($_SERVER["HTTP_USER_AGENT"],"Windows CE"))
      {
        $extrainfo = substr($_SERVER["HTTP_USER_AGENT"], strpos($_SERVER["HTTP_USER_AGENT"],"("));
        $seperated = explode(";",substr($extrainfo,0,strlen($extrainfo)-1));
        foreach ($seperated as $key => $info)
        {
          // We trim the whitespaces
          $seperated[$key]= $info = trim($info);

          // And check for size
          $explodedinfo = explode("x",$info);
          $size = array();
          if ($explodedinfo[0] && is_numeric($explodedinfo[0]) && $explodedinfo[1] && is_numeric($explodedinfo[1]))
          {
            // We've got browser size
            $size["width"]  = $explodedinfo[0];
            $size["height"] = $explodedinfo[1];
          }
        }
        return $size;
      }
      else
        return false;
    }

    function detectOS()
    {
     // Windows 3.x
      if( preg_match('Win16',$this->ua) || preg_match('windows 3.1',$this->ua) || preg_match('windows 16-bit',$this->ua) || preg_match('16bit',$this->ua))
      {
       $this->platform = "Win16";
       $this->os       = "Win31";
       $this->osname   = "Windows 3.x";
      }

     // Windows 95
      if(preg_match('Win95',$this->ua) || preg_match('windows 95',$this->ua))
      {
       $this->platform = "Win32";
       $this->os       = "Win95";
       $this->osname   = "Windows 95";
      }

     // Windows 98
      elseif(preg_match('Win98',$this->ua) || preg_match('windows 98',$this->ua))
      {
       $this->platform = "Win32";
       $this->os       = "Win98";
       $this->osname   = "Windows 98";
      }

     // Windows NT
      elseif(preg_match('WinNT',$this->ua) || preg_match('windows NT',$this->ua))
      {
       $this->platform = "Win32";
       $this->os       = "WinNT";
       $this->osname   = "Windows NT";

       if(preg_match('NT 5.0',$this->ua))
       {
        $this->platform = "Win32";
        $this->os       = "Win2000";
        $this->osname   = "Windows 2000";
       }

       if(preg_match('NT 5.1',$this->ua))
       {
        $this->platform = "Win32";
        $this->os       = "WinXP";
        $this->osname   = "Windows XP";
       }
      }

     // Windows ME
      elseif(preg_match('win 9x 4.90',$this->ua))
      {
       $this->platform = "Win32";
       $this->os       = "Win ME";
       $this->osname   = "Windows Millenium";
      }

     // Other Win 32
      elseif(preg_match('Win',$this->ua))
      {
       $this->platform = "Win32";
       $this->os = "Win9xNT";
      }

     // Check for os/2
      if( preg_match('os/2',$this->ua) || preg_match('ibm-webexplorer',$this->ua))
      {
       $this->platform = "OS/2";
       $this->os = "OS/2";
      }

     // Check for Mac 68000
      if( preg_match('68k',$this->ua) || preg_match('68000',$this->ua))
      {
       $this->platform = "Mac";
       $this->os = "Mac68k";
      }

     // Check for Mac PowerPC
      if( preg_match('ppc',$this->ua) || preg_match('powerpc',$this->ua))
      {
       $this->platform = "Mac";
       $this->os = "MacPPC";
      }

     // SunOS (All versions)
      if(preg_match('sunos',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "sun";
      }

     // SUN versions
      if(preg_match('sunos 4',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "SUN 4";
      }
      elseif(preg_match('sunos 5',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "SUN 5";
      }
      elseif(preg_match('i86',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "SUN i86";
      }

     // Irix
      if(preg_match('irix',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Irix";
      }
      if(preg_match('irix 6',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Irix 6";
      }
      elseif(preg_match('irix 5',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Irix 5";
      }

     // HP-UX
      if(preg_match('hp-ux',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "HP UX";
      }
      if(preg_match('hp-ux',$this->ua) && preg_match('10.',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "HP UX 10.x";
      }
      elseif(preg_match('hp-ux',$this->ua) && preg_match('09.',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "HP UX 9.x";
      }

     //AIX
      if(preg_match('aix',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "IBM AIX";
      }
      if(preg_match('aix1',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "IBM AIX 1.x";
      }
      elseif(preg_match('aix2',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "IBM AIX 2.x";
      }
      elseif(preg_match('aix3',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "IBM AIX 3.x";
      }
      elseif(preg_match('aix4',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "IBM AIX 4.x";
      }

     // Linux
      if(preg_match('inux',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Linux";
      }

     // Unixware
      if(preg_match('unix_system_v',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "UnixWare";
      }

      // mpras
      if(preg_match('ncr',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "NCR mpras";
      }

      // Reliant
      if(preg_match('reliantunix',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Reliant";
      }

      // DEC
      if(preg_match('dec',$this->ua) || preg_match('osfl',$this->ua) || preg_match('alphaserver',$this->ua) || preg_match('ultrix',$this->ua)  || preg_match('alphastation',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "DEC";

       if (preg_match('ultrix',$this->ua))
       {
        $this->os = "DEC Ultrix";
       }
      }

     // Sinix
      if(preg_match('sinix',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "Sinix";
      }

     // BSD
      if(preg_match('bsd',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "BSD";
      }

     // FreeBSD
      if(preg_match('freebsd',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "FreeBSD";
      }

      // VMS
      if(preg_match('vax',$this->ua) || preg_match('openvms',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "VAX VMS";
      }

      // SCO
      if(preg_match('sco ',$this->ua) || preg_match('unix_sv',$this->ua))
      {
       $this->platform = "Unix";
       $this->os = "SCO";
      }
    }
 }

 function getBrowserInfo($useragent="")
 {
      $tmp = new browserInfo($useragent);
      return array
          (
           "ua"=>$tmp->ua,
           "version"=>$tmp->full_version,
           "browser"=>$tmp->browser,
           "major"=>$tmp->major,
           "minor"=>$tmp->minor,
           "os"=>$tmp->os,
           "platform"=>$tmp->platform,
           "short"=>$tmp->short,
           "brName"=>$tmp->brName,
           "osName"=>$tmp->osName,
           "hasGui"=>$tmp->hasGui,
           "spider"=>$tmp->spider,
           "family"=>$tmp->family,
           "gecko"=>$tmp->gecko
          );

 }

 /**
   * This function can be used to determine if we are using ie specific extensions.
   * The function checks the value of the $config_enable_ie_extensions variable
   * and also the browser that the user is using.
   */
  function ieExtensions()
  {
    $browser = getBrowserInfo();
    return (atkconfig("enable_ie_extensions") && $browser["family"]=="ie5up");
  }

?>
