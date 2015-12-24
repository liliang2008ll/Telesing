/*
 * 	   Created by Daniel Nadeau
 * 	   daniel.nadeau01@gmail.com
 * 	   danielnadeau.blogspot.com
 * 
 * 	   Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.
 */

package net.telesing.tscom.view.draw;

import android.content.Context;
import android.graphics.*;
import android.graphics.Bitmap.Config;
import android.graphics.Paint.Align;
import android.graphics.Path.Direction;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import net.telesing.tscom.common.utils.DisplayUtil;

import java.util.ArrayList;

public class LineGraph extends View {

	private ArrayList<Line> lines = new ArrayList<Line>();
	private Paint paint = new Paint();
	private Paint txtPaint = new Paint();
	private float minY = 0, minX = 0;
	private float maxY = 0, maxX = 0;
	private boolean isMaxYUserSet = false;
	private int lineToFill = -1;
	private int indexSelected = -1;
	private Bitmap fullImage;
	private boolean shouldUpdate = false;
	private boolean showMinAndMax = false;
	private boolean showHorizontalGrid = false;
	private int gridColor = 0xffffffff;
	private int labelSize = 10;

	public LineGraph(Context context){
		this(context,null);
	}
	public LineGraph(Context context, AttributeSet attrs) {
		super(context, attrs);
		txtPaint.setColor(0xffffffff);
		txtPaint.setTextSize(20);
		txtPaint.setAntiAlias(true);
	}
	public void setGridColor(int color)
	{
		gridColor = color;
	}
	public void showHorizontalGrid(boolean show)
	{
		showHorizontalGrid = show;
	}
	public void showMinAndMaxValues(boolean show)
	{
		showMinAndMax = show;
	}
	public void setTextColor(int color)
	{
		txtPaint.setColor(color);
	}
	public void setTextSize(float s)
	{
		txtPaint.setTextSize(s);
	}
	public void setMinY(float minY){
		this.minY = minY;
	}

	public void update()
	{
		shouldUpdate = true;
		postInvalidate();
	}
	public void removeAllLines(){
		while (lines.size() > 0){
			lines.remove(0);
		}
		shouldUpdate = true;
		postInvalidate();
	}

	public void addLine(Line line) {
		lines.add(line);
		shouldUpdate = true;
		postInvalidate();
	}

	public void addOneLine(Line line) {
		lines = new ArrayList<Line>();
		lines.add(line);
		shouldUpdate = true;
		postInvalidate();
	}
	public ArrayList<Line> getLines() {
		return lines;
	}
	public void setLineToFill(int indexOfLine) {
		this.lineToFill = indexOfLine;
		shouldUpdate = true;
		postInvalidate();
	}
	public int getLineToFill(){
		return lineToFill;
	}
	public void setLines(ArrayList<Line> lines) {
		this.lines = lines;
	}
	public Line getLine(int index) {
		return lines.get(index);
	}
	public int getSize(){
		return lines.size();
	}

	public void setRangeY(float min, float max) {
		minY = min;
		maxY = max;
		isMaxYUserSet = true;
	}
	public float getMaxY(){
		if (isMaxYUserSet){
			return maxY;
		} else {
			maxY = lines.get(0).getPoint(0).getY();
			for (Line line : lines){
				for (LinePoint point : line.getPoints()){
					if (point.getY() > maxY){
						maxY = point.getY();
					}
				}
			}
			return maxY;
		}

	}
	public float getMinY(){
		if (isMaxYUserSet){
			return minY;
		} else {
			float min = lines.get(0).getPoint(0).getY();
			for (Line line : lines){
				for (LinePoint point : line.getPoints()){
					if (point.getY() < min) min = point.getY();
				}
			}
			minY = min;
			return minY;
		}
	}
	public float getMaxX(){
		float max = lines.get(0).getPoint(0).getX();
		for (Line line : lines){
			for (LinePoint point : line.getPoints()){
				if (point.getX() > max) max = point.getX();
			}
		}
		maxX = max;
		return maxX;

	}
	public float getMinX(){
		float max = lines.get(0).getPoint(0).getX();
		for (Line line : lines){
			for (LinePoint point : line.getPoints()){
				if (point.getX() < max) max = point.getX();
			}
		}
		maxX = max;
		return maxX;
	}

	public void onDraw(Canvas ca) {
		if (fullImage == null || shouldUpdate) {
			fullImage = Bitmap.createBitmap(getWidth(), getHeight(), Config.ARGB_8888);
			Canvas canvas = new Canvas(fullImage);
			String max = (int)maxY+"";// used to display max
			String min = (int)minY+"";// used to display min
			paint.reset();
			Path path = new Path();

			float bottomPadding = 0, topPadding = DisplayUtil.getPx(getContext(),17);
			float sidePadding = 0;
			if (this.showMinAndMax)
				sidePadding = txtPaint.measureText(max);
			if (labelSize > bottomPadding) {
				bottomPadding = labelSize;
			}

			float usableHeight = getHeight() - bottomPadding - topPadding;
			float usableWidth = getWidth() - sidePadding*2;
			float lineSpace = usableHeight/10;



			paint.reset();

			paint.setColor(this.gridColor);
			paint.setAlpha(50);
			paint.setAntiAlias(true);
			canvas.drawLine(sidePadding, getHeight() - bottomPadding, getWidth(), getHeight()-bottomPadding, paint);
			if(this.showHorizontalGrid)
				for(int i=1;i<=10;i++)
				{
					canvas.drawLine(sidePadding, getHeight() - bottomPadding-(i*lineSpace), getWidth(), getHeight()-bottomPadding-(i*lineSpace), paint);
				}
			paint.setAlpha(255);


			paint.setTextAlign(Align.CENTER);
			paint.setTextSize(labelSize);

			for (Line line : lines){
				int count = 0;
				float lastXPixels = 0, newYPixels;
				float lastYPixels = 0, newXPixels;
				float maxY = getMaxY();
				float minY = getMinY();
				float maxX = getMaxX();
				float minX = getMinX();

				paint.setColor(line.getColor());
				paint.setStrokeWidth(2);

				for (LinePoint p : line.getPoints()){
					float yPercent = (p.getY()-minY)/(maxY - minY);
					float xPercent = (p.getX()-minX)/(maxX - minX);
					if (count == 0){
						lastXPixels = sidePadding + (xPercent*usableWidth);
						lastYPixels = getHeight() - bottomPadding - (usableHeight*yPercent);
					} else {
						newXPixels = sidePadding + (xPercent*usableWidth);
						newYPixels = getHeight() - bottomPadding - (usableHeight*yPercent);
//						Log.e("======>",lastXPixels+","+lastYPixels+","+ newXPixels+","+ newYPixels);
						canvas.drawLine(lastXPixels, lastYPixels, newXPixels, newYPixels, paint);
						lastXPixels = newXPixels;
						lastYPixels = newYPixels;
					}
					if (p.getLabel_string()!=null) {
						canvas.drawText(p.getLabel_string(), lastXPixels, usableHeight+bottomPadding, paint);
					}
					count++;
				}
			}




			shouldUpdate = false;
			if (this.showMinAndMax) {
				ca.drawText(max, 0, txtPaint.getTextSize(), txtPaint);
				ca.drawText(min,0,this.getHeight(),txtPaint);
			}
		}

		ca.drawBitmap(fullImage, 0, 0, null);


	}





	public int getLabelSize() {
		return labelSize;
	}
	public void setLabelSize(int labelSize) {
		this.labelSize = labelSize;
	}

	public interface OnPointClickedListener {
		abstract void onClick(int lineIndex, int pointIndex);
	}
}
