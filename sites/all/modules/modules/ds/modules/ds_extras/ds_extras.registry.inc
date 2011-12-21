<?php

/**
 * @file
 * Display suite Extras registry file.
 */

/**
 * Implements hook_menu().
 */
function _ds_extras_menu() {
  $items = array();

  $items['admin/structure/ds/extras'] = array(
    'title' => 'Extras',
    'description' => 'Configure extra functionality for Display Suite.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('ds_extras_settings'),
    'access arguments' => array('admin_display_suite'),
    'file' => 'ds_extras.admin.inc',
  );

  if (variable_get('ds_extras_switch_field')) {
    $items['ds-switch-view-mode'] = array(
      'title' => 'Switch view',
      'description' => 'Switches a view mode inline.',
      'page callback' => 'ds_switch_view_mode_inline',
      'access arguments' => array('access content'),
      'file' => 'ds_extras.pages.inc',
      'type' => MENU_CALLBACK,
    );
  }

  if (variable_get('ds_extras_vd', FALSE) && module_exists('field_ui') && module_exists('views')) {
    $items['admin/structure/ds/vd'] = array(
      'title' => 'Views displays',
      'description' => 'Manage your views templates.',
      'page callback' => 'ds_extras_vd',
      'file' => 'ds_extras.vd.inc',
      'access arguments' => array('admin_display_suite'),
    );
  }

  if (variable_get('ds_extras_contextual', FALSE) && module_exists('contextual') && module_exists('field_ui')) {
    $items['node/%node/display'] = array(
      'title' => 'Manage display',
      'description' => 'Manage display of this content.',
      'page callback' => 'ds_extras_contextual_page_tab',
      'page arguments' => array(1, 'node'),
      'file' => 'ds_extras.pages.inc',
      'access arguments' => array('administer content types'),
      'type' => MENU_LOCAL_TASK,
    );

    $items['user/%user/display'] = array(
      'title' => 'Manage display',
      'description' => 'Manage display of this user profile.',
      'page callback' => 'ds_extras_contextual_page_tab',
      'page arguments' => array(1, 'user'),
      'file' => 'ds_extras.pages.inc',
      'access arguments' => array('administer users'),
      'type' => MENU_LOCAL_TASK,
    );

    if (module_exists('taxonomy')) {
      $items['taxonomy/term/%taxonomy_term/display'] = array(
        'title' => 'Manage display',
        'description' => 'Manage display of this term.',
        'page callback' => 'ds_extras_contextual_page_tab',
        'page arguments' => array(2, 'taxonomy_term'),
        'access arguments' => array('administer taxonomy'),
        'type' => MENU_LOCAL_TASK,
        'weight' => 11,
        'file' => 'ds_extras.pages.inc',
      );
    }
  }

  return $items;
}

/**
 * Implements hook_menu_alter().
 */
function _ds_extras_menu_alter(&$items) {
  $items['node/%node']['page callback'] = 'ds_extras_node_show';
  $items['node/%node']['file'] = 'ds_extras.pages.inc';
  $items['node/%node']['file path'] = drupal_get_path('module', 'ds_extras');
}

/**
 * Implements hook_entity_info().
 */
function _ds_extras_entity_info() {

  if (!variable_get('ds_extras_vd', FALSE)) {
    return;
  }

  $bundles = array();
  ctools_include('export');
  $vd_settings = ctools_export_crud_load_all('ds_vd');
  foreach ($vd_settings as $key => $vd) {
    $bundles[$vd->vd] = array(
      'label' => check_plain($vd->label),
      'admin' => array('path' => 'admin/structure/ds/vd/' . $vd->vd),
    );
  }

  // Register a views entity on behalf of Views.
  $return = array(
    'ds_views' => array(
      'label' => t('Display suite Views'),
      'bundles' => $bundles,
      'ds_display' => TRUE,
    ),
  );

  return $return;
}


/**
 * Implements hook_theme_registry_alter().
 */
function _ds_extras_theme_registry_alter(&$theme_registry) {

  // Add views preprocess layout.
  if (variable_get('ds_extras_vd', FALSE)) {
    $theme_registry['views_view']['preprocess functions'][] = 'ds_extras_preprocess_view_layout';
  }

  // Add process page function.
  if (variable_get('ds_extras_hide_page_title', FALSE)) {
    $theme_registry['page']['process functions'][] = 'ds_extras_process_page_title';
  }

  // Change the default field theming function.
  if (variable_get('ds_extras_field_template', FALSE)) {

    // We change the default theme_field function by ours.
    $theme_registry['field']['function'] = 'theme_ds_field';

    // Kill all theme hook suggestions.
    if (variable_get('ft-kill', FALSE)) {
      foreach ($theme_registry as $key => $hook) {
        if (isset($hook['base hook']) && $hook['base hook'] == 'field') {
          unset($theme_registry[$key]);
        }
      }
    }
  }
}

/**
 * Implements hook_module_implements_alter().
 */
function _ds_extras_module_implements_alter(&$implementations, $hook) {

  // Because it's possible to turn on/off features for DS extras,
  // we'll unset hooks here if necessary which otherwhise do nothing at all.

  // Field template
  $fs_hooks = array(
    'ds_field_settings_alter',
    'form_ds_styles_form_alter',
  );
  if (!variable_get('ds_extras_field_template', FALSE) && in_array($hook, $fs_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Contextual
  $fs_hooks = array(
    'contextual_links_view_alter',
  );
  if ((!variable_get('ds_extras_contextual', FALSE) && in_array($hook, $fs_hooks)) || !module_exists('field_ui')) {
    unset($implementations['ds_extras']);
  }

  // Region to block
  $region_hooks = array(
    'ds_layout_region_alter',
    'field_attach_view_alter',
    'block_info',
    'block_view'
  );
  if (!variable_get('ds_extras_region_to_block', FALSE) && in_array($hook, $region_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Hide page title
  $hide_page_title_hooks = array(
    'ds_layout_settings_alter',
  );
  if (!variable_get('ds_extras_hide_page_title', FALSE) && in_array($hook, $hide_page_title_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Switch view mode
  $switch_hooks = array(
    'menu_alter',
    'permission',
    'form_node_form_alter',
  );
  if (!variable_get('ds_extras_switch_view_mode', FALSE) && in_array($hook, $switch_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Fivestar
  $fivestar_hooks = array(
    'form_fivestar_node_type_tag_form_alter',
    'fivestar_access',
  );
  if (!variable_get('ds_extras_fivestar', FALSE) && in_array($hook, $fivestar_hooks)) {
    unset($implementations['ds_extras']);
  }

  // Views displays
  $vd_hooks = array(
    'entity_info',
    'ctools_plugin_api',
  );
  if (!variable_get('ds_extras_vd', FALSE) && in_array($hook, $vd_hooks)) {
    unset($implementations['ds_extras']);
  }
}
