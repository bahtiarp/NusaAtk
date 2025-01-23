<?php /* Smarty version 2.6.31, created on 2024-11-14 05:15:01
         compiled from ./themes/nusaatk/templates/box.tpl */ ?>
<!--<div class="newboxtitle"><?php echo $this->_tpl_vars['title']; ?>
</div>-->
<!--<div class="content-wrapper" style="min-height: 1969px;">-->


<div class="app-content-header"> <!--begin::heaader-->
    <div class="container-fluid">
        <div class="row">
            <div class="col-sm-6">
                <h3 class="mb-0"><?php echo $this->_tpl_vars['title']; ?>
</h3>
            </div>
            <div class="col-sm-6">
                <ol class="breadcrumb justify-content-xl-end">
                    <li class="breadcrumb   breadcrumb-item"><a href="<?php echo $this->_tpl_vars['stacktrace'][0]['url']; ?>
"><span
                                class="fa-solid fa-house"></span></a></li>
                </ol>

            </div>
        </div>
    </div>
</div> <!--end::header-->

<div class="app-content">
    <div class="container-fluid">
                <?php echo $this->_tpl_vars['content']; ?>

    </div>
</div>