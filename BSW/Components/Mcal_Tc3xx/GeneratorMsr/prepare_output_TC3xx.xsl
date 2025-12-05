<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output indent="yes"/>
	<!--
	-->
	<xsl:variable name="PluginNameDem" select="'AURIX2G/EcucDefs'"/>
	<xsl:variable name="PluginNameEcum" select="'AURIX2G/EcucDefs'"/>
	<xsl:variable name="PluginNameEcuc" select="'AURIX2G/EcucDefs'"/>
	<!--
	-->
	<!-- ##### Template to remove all MICROSAR components for Tresos Generation ##### -->
	<xsl:template match="*[(local-name()='MODULE-CONFIGURATION' or local-name()='ECUC-MODULE-CONFIGURATION-VALUES') and 
	                       ./*[local-name()='DEFINITION-REF' and contains(text(),'/MICROSAR/')] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/EcuM'))] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/EcuC'))] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/Dem'))]]"/>
	<xsl:template match="*[(local-name()='MODULE-CONFIGURATION' or local-name()='ECUC-MODULE-CONFIGURATION-VALUES') and 
	                       ./*[local-name()='DEFINITION-REF' and contains(text(),'/AUTOSAR/')] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/EcuM'))] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/EcuC'))] and 
	                       ./*[local-name()='DEFINITION-REF' and not (contains(text(),'/Dem'))]]"/>
	
	<!-- ##### Template to remove all ECUC-MODULE-CONFIGURATION-VALUESs except the Mcal relevant ##### -->
	<xsl:template match="*[local-name()='ECUC-MODULE-CONFIGURATION-VALUES' and 
							./*[local-name()='DEFINITION-REF' and 
							not(substring-after(text(), substring-before(text(), '/EcuM'))='/EcuM') and
							not(substring-after(text(), substring-before(text(), '/EcuC'))='/EcuC') and
							not(substring-after(text(), substring-before(text(), '/Dem'))='/Dem') and
							not(contains(text(),'/AURIX2G/EcucDefs/ResourceM')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Adc')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Crc')) and
							not(contains(text(),'/AURIX2G/EcucDefs/Dio')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Dma')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Dsadc')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Fee')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Fls')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/FlsLoader')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Gpt')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Icu')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Irq')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/McalLib')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Mcu')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Ocu')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Port')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Pwm')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Smu')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Spi')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Uart')) and 
							not(contains(text(),'/AURIX2G/EcucDefs/Wdg'))]]"/>
	<!-- ##### MODULE-REFS will not be removed but are ignored by Tresos ##### -->
	<!--<xsl:template match="*[local-name()='MODULE-REFS' and *[local-name()='MODULE-REF' and contains(text(),'/MICROSAR/')]]"/>-->
<!-- ##### Template to remove EcuC if it is not part of the Mcal - shall be commented if EcuC exists##### -->
<!-- Prepare EcuC --><!--
	<xsl:template match="*[local-name()='ECUC-MODULE-CONFIGURATION-VALUES' and 
							./*[local-name()='DEFINITION-REF' and 
							(substring-after(text(), substring-before(text(), '/EcuC'))='/EcuC')]]"/>
--><!-- End of Prepare EcuC -->
	<!--
	-->
	<!-- ##### Template to copy all other nodes (with attributes) ##### -->
	<xsl:template match="*" priority="0" name="copy_cat">
		<xsl:copy>
			<xsl:for-each select="@*">
				<xsl:attribute name="{local-name(.)}"><xsl:value-of select="."/></xsl:attribute>
			</xsl:for-each>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>
	<!--
	-->
	<!-- ########## The few following lines are necessary to adapt the MICROSAR Dem to Infineons Dem-Stub ########## -->
	<!-- ##### 'Rename MICROSAR-Dem to /$PluginName$/Dem ##### -->
	<xsl:template match="*[local-name()='DEFINITION-REF' and contains(., '/Dem')]" priority="1">
		<xsl:copy>
			<xsl:for-each select="@*">
				<xsl:attribute name="{local-name(.)}"><xsl:value-of select="."/></xsl:attribute>
			</xsl:for-each>
			<xsl:text>/</xsl:text>
			<xsl:value-of select="$PluginNameDem"/>
			<xsl:text>/Dem</xsl:text>
			<xsl:value-of select="substring-after(., '/Dem')"/>
		</xsl:copy>
	</xsl:template>
	<!--
	-->
	<!-- ########## The few following lines are necessary to adapt the MICROSAR EcuC to Infineons EcuC-Stub ########## -->
	<!-- ##### 'Rename MICROSAR-EcuC to /$PluginName$/EcuC ##### -->
	<xsl:template match="*[local-name()='DEFINITION-REF' and contains(., '/EcuC')]" priority="1">
		<xsl:copy>
			<xsl:for-each select="@*">
				<xsl:attribute name="{local-name(.)}"><xsl:value-of select="."/></xsl:attribute>
			</xsl:for-each>
			<xsl:text>/</xsl:text>
			<xsl:value-of select="$PluginNameEcuc"/>
			<xsl:text>/EcuC</xsl:text>
			<xsl:value-of select="substring-after(., '/EcuC')"/>
		</xsl:copy>
	</xsl:template>
	<!--
	-->
	<!-- ########## The few following lines are necessary to adapt the MICROSAR EcuM to Infineons EcuM-Stub ########## -->
	<!-- ##### 'Rename MICROSAR-EcuM to /$PluginName$/EcuM ##### -->
	<xsl:template match="*[local-name()='DEFINITION-REF' and contains(., '/EcuM')]" priority="1">
		<xsl:copy>
			<xsl:for-each select="@*">
				<xsl:attribute name="{local-name(.)}"><xsl:value-of select="."/></xsl:attribute>
			</xsl:for-each>
			<xsl:text>/</xsl:text>
			<xsl:value-of select="$PluginNameEcum"/>
			<xsl:text>/EcuM</xsl:text>
			<xsl:value-of select="substring-after(., '/EcuM')"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
