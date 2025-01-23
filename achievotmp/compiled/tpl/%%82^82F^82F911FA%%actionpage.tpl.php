<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:01
         compiled from ./themes/nusaatk/templates/actionpage.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkmessages', './themes/nusaatk/templates/actionpage.tpl', 1, false),)), $this); ?>
<?php echo smarty_function_atkmessages(array(), $this);?>

<?php if (count ( $this->_tpl_vars['atkmessages'] )): ?>
<div style='position:fixed;left:40%;top:150px;z-index:9999;margin-right:10px;' class="atkmessages">
  <?php $_from = $this->_tpl_vars['atkmessages']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['message']):
?>
    <div class="atkmessages"><?php echo $this->_tpl_vars['message']['message']; ?>
</div>
  <?php endforeach; endif; unset($_from); ?>
</div>
     <script>fade("atkmessages");</script>
<?php endif; ?>

<?php $_from = $this->_tpl_vars['blocks']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['block']):
?>
  <?php echo $this->_tpl_vars['block']; ?>

<?php endforeach; endif; unset($_from); ?>