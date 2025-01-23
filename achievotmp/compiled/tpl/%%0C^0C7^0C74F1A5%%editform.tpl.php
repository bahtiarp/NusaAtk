<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:01
         compiled from ./themes/nusaatk/templates/editform.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atktext', './themes/nusaatk/templates/editform.tpl', 5, false),)), $this); ?>
<?php if (( count ( $this->_tpl_vars['errors'] ) > 0 )): ?>

        <?php echo $this->_tpl_vars['errortitle']; ?>
<br/>
        <?php $_from = $this->_tpl_vars['errors']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['error']):
?>
          <?php echo $this->_tpl_vars['error']['label']; ?>
: <?php echo $this->_tpl_vars['error']['message']; ?>
 <?php if ($this->_tpl_vars['error']['tablink']): ?> (<?php echo smarty_function_atktext(array('id' => 'error_tab'), $this);?>
 <?php echo $this->_tpl_vars['error']['tablink']; ?>
)<?php endif; ?><br/>
        <?php endforeach; endif; unset($_from); ?>

  <?php endif; ?>

<div class="row">
<div class="col col-sm-6">
  <?php $_from = $this->_tpl_vars['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <?php if ($this->_tpl_vars['field']['column'] != 1): ?><?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "theme:field.tpl", 'smarty_include_vars' => array('field' => $this->_tpl_vars['field'])));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?><?php endif; ?>
  <?php endforeach; endif; unset($_from); ?>
</div>
<div class="col col-sm-6">
  <?php $_from = $this->_tpl_vars['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field']):
?>
    <?php if ($this->_tpl_vars['field']['column'] == 1): ?><?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => "theme:field.tpl", 'smarty_include_vars' => array('field' => $this->_tpl_vars['field'])));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?><?php endif; ?>
  <?php endforeach; endif; unset($_from); ?>
</div>
</div>