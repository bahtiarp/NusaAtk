<?php

/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * This file is the skeleton index file, which you can copy to your
 * application dir and modify if necessary. By default, it checks
 * the setting of $config_fullscreen, and if set, launches the
 * app in a full screen window. If not set, the frameset is loaded.
 *
 * @package atk
 * @subpackage skel
 *
 * @author Ivo Jansch <ivo@achievo.org>
 *
 * @copyright (c)2000-2004 Ibuildings.nl BV
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 *
 * @version $Revision: 5718 $
 * $Id: index.php 5718 2007-05-04 13:30:55Z ivo $
 */

/**
 * @internal includes
 */
$config_atkroot = "./";
include_once "atk.inc";

atksession();
atksecure();

$theme = atkinstance('atk.ui.atktheme');
$indexpage = atknew('atk.ui.atkindexpage');
$destination = array("atknodetype" => "welcome.dashboard", "atkaction" => "dashboard");
if (atkGetUser()["name"]=="administrator") {
  $indexpage->setDefaultDestination($destination);
}

$indexpage->generate();


