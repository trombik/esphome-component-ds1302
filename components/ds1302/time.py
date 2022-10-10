import esphome.config_validation as cv
import esphome.codegen as cg
from esphome import automation
from esphome.components import time
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_CLK_PIN,
    CONF_DIO_PIN,
    CONF_CS_PIN,
)


CODEOWNERS = ["@trombik"]
DEPENDENCIES = ["time"]
ds1302_ns = cg.esphome_ns.namespace("ds1302")
DS1302Component = ds1302_ns.class_("DS1302Component", time.RealTimeClock)
WriteAction = ds1302_ns.class_("WriteAction", automation.Action)
ReadAction = ds1302_ns.class_("ReadAction", automation.Action)


CONFIG_SCHEMA = time.TIME_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(DS1302Component),
        cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_DIO_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_CS_PIN): pins.gpio_output_pin_schema,
    }
)


@automation.register_action(
    "ds1302.write_time",
    WriteAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(DS1302Component),
        }
    ),
)
async def ds1302_write_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var


@automation.register_action(
    "ds1302.read_time",
    ReadAction,
    automation.maybe_simple_id(
        {
            cv.GenerateID(): cv.use_id(DS1302Component),
        }
    ),
)
async def ds1302_read_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await time.register_time(var, config)

    clk = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(clk))

    dio = await cg.gpio_pin_expression(config[CONF_DIO_PIN])
    cg.add(var.set_dio_pin(dio))

    cs = await cg.gpio_pin_expression(config[CONF_CS_PIN])
    cg.add(var.set_cs_pin(cs))
