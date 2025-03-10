<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage include
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: $
   * $Id: configs.inc 6771 2010-03-12 09:44:28Z peter $
   */
  require_once($GLOBALS['config_atkroot']."atk/class.atkconfig.inc");

  atkConfig::loadGlobals();

  // Auto-register modules
  if (file_exists("{$GLOBALS['config_module_path']}/config.modules.inc"))
  {
    include_once("{$GLOBALS['config_module_path']}/config.modules.inc");
  }
?>
